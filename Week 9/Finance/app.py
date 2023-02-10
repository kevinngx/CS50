import os
import requests
from datetime import datetime


from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd



# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Need to create addition tables: transactions and portfolio
# db.execute("CREATE TABLE IF NOT EXISTS transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER NOT NULL, transaction_type TEXT NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL, price REAL NOT NULL, transacted TEXT NOT NULL)" );
# db.execute("CREATE TABLE IF NOT EXISTS portfolio (user_id INTEGER NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL)" );


# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

API_KEY = os.environ.get("API_KEY")
# API_KEY = "pk_5eb922146e3842ef8f05f533220d7ed4pk_5eb922146e3842ef8f05f533220d7ed4"


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get user cash total
    result = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = result[0]['cash']

    # pull all transactions belonging to user
    portfolio = db.execute("SELECT symbol, shares FROM portfolio where user_id = ?", session["user_id"])

    # if not portfolio:
    #     return apology("sorry you have no holdings")

    # Starting balance = cash, to add individual stock values
    grand_total = cash

    # determine current price, stock total value and grand total value
    for stock in portfolio:
        stock_lookup = lookup(stock["symbol"])
        price = stock_lookup['price']
        total = stock["shares"] * price
        stock.update({'price': price, 'total': total})
        grand_total += total

    return render_template("index.html", stocks=portfolio, cash=cash, total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

         # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must provide shares", 400)

        # Check is int
        if request.form.get("shares").isdigit() == False:
            return apology("shares input must be a whole number", 400)

        # Ensure shares submitted is positive
        if int(request.form.get("shares")) <= 0:
            return apology("shares cannot be negative or zero", 400)

        # Define variables for simpler use
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Request stock info
        stock = request.form.get("symbol")
        stock_lookup = lookup(stock)

        if stock_lookup == None:
            return apology("Stock symbol is not valid, please try again", 400)

        price = stock_lookup['price']
        print(f'price = {price}')
        print(f'transaction value = {price} * {shares} = {price * shares}')

        # Check if user has enough $$$
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        print(f'User balance is {rows[0]["cash"]}')

        if rows[0]["cash"] < (price * shares):
            return apology("insufficient funds", 403)

        # Execute transaction
        execute_transaction(session["user_id"], "buy", symbol, shares, price)

        # Redirect user to home page
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    response = db.execute(\
        "SELECT transaction_type, symbol, shares, price, transacted FROM transactions WHERE user_id = ? ORDER BY transacted desc", session["user_id"])
    print(response)

    return render_template("history.html", transactions=response)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Get Stock Info
        stock = request.form.get("symbol")
        stock_lookup = lookup(stock)

        if stock_lookup == None:
            return apology("Stock symbol is not valid, please try again", 400)

        company_name = stock_lookup['name']
        company_symbol = stock_lookup['symbol']
        price = stock_lookup['price']
        output = f'A share of {company_name} ({company_symbol}) costs{usd(price)}'

        return render_template("quoted.html", stock_quote=output)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        # Ensure password and confirmation matches
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Check Inputs

        username = request.form.get("username")
        password = request.form.get("password")
        password_hash = generate_password_hash(password)

        print(f'Username = {username}')
        print(f'Password = {password}')
        print(f'Password_Hash = {password_hash}')

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Check if username is already taken
        if len(rows) == 1:
            return apology("username is already taken", 400)

        # Insert new user into database
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, password_hash)

        # Remember which user has logged in
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]
        print(f'Session set to user id = {session["user_id"]}')

        # Redirect user to home page
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check is int
        if request.form.get("shares").isdigit() == False:
            return apology("shares input must be a whole number", 400)

        # Ensure shares submitted is positive
        if int(request.form.get("shares")) <= 0:
            return apology("shares cannot be negative or zero", 400)

        # Check if user has enough shares
        available_shares = db.execute("SELECT shares FROM portfolio where user_id = ? and symbol = ?",\
             session["user_id"], request.form.get("symbol"))[0]['shares']
        selling_shares = int(request.form.get("shares") )
        print(f'Shares available = {available_shares}, shares being sold = {selling_shares}')

        if selling_shares > available_shares:
            return apology("insufficcient shares in portfolio", 400)

        # Get share price
        stock_lookup = lookup(request.form.get("symbol"))
        price = stock_lookup['price']
        execute_transaction(session["user_id"], "sell", request.form.get("symbol"), selling_shares, price)

        # Redirect user to home page
        return redirect("/")

    # Get portfolio of stocks
    response = db.execute("SELECT symbol, shares FROM portfolio where user_id = ?", session["user_id"])
    return render_template("sell.html", portfolio=response)


def execute_transaction(user, transaction_type, symbol, shares, price):
    # execute buy_transaction
    if transaction_type == "buy":
        # Create new transaction
        db.execute("INSERT INTO transactions (user_id, transaction_type, symbol, shares, price, transacted) VALUES (?,?,?,?,?,?)",\
                   user, transaction_type, symbol, shares, price, datetime.now())

        # Update portfolio entryexport API_KEY=
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", user, symbol)
        if len(rows) == 1:
            db.execute("UPDATE portfolio set shares = ?", (rows[0]["shares"] + shares))
        else:
            db.execute("INSERT INTO portfolio (user_id, symbol, shares) VALUES (?,?,?)", user, symbol.upper(), shares)

        # Update user wallet
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        db.execute("UPDATE users set cash = ?", (rows[0]["cash"] - (shares * price)))

    if transaction_type == "sell":
        # Create new transaction
        db.execute( "INSERT INTO transactions (user_id, transaction_type, symbol, shares, price, transacted) VALUES (?,?,?,?,?,?)",\
                   user, transaction_type, symbol, shares, price, datetime.now())

        # Update portfolio entryexport API_KEY=
        rows = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", user, symbol)
        db.execute("UPDATE portfolio set shares = ? where symbol = ?", (rows[0]["shares"] - shares), symbol)

        # Update user wallet
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        db.execute("UPDATE users set cash = ?", (rows[0]["cash"] + (shares * price)))