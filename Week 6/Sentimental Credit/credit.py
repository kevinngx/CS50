import sys

while True:
    user_input = input("Number: ")
    try:
        if len(user_input) > 0 and int(user_input):
            credit_card = user_input
            break
    except ValueError:
        continue

if len(credit_card) < 13 or 16 < len(credit_card):
    print("INVALID")

else:
    even = 0
    odd = 0
    card_length = len(credit_card)

    if card_length % 2 == 0:
        for i in range(card_length):
            num = int(credit_card[i])
            if i % 2 == 0:
                multiple = num * 2
                if multiple >= 10:
                    even += multiple // 10
                    even += multiple % 10
                else:
                    even += multiple
            else:
                odd += num
    else:
        for i in range(card_length):
            num = int(credit_card[i])
            if i % 2 != 0:
                multiple = num * 2
                if multiple >= 10:
                    even += multiple // 10
                    even += multiple % 10
                else:
                    even += multiple
            else:
                odd += num

    check = (even + odd) % 10

    if check == 0:
        first_digit = int(credit_card[0])
        second_digit = int(credit_card[1])
        if first_digit == 3 and second_digit == 4 or second_digit == 7:
            print("AMEX")
        elif first_digit == 5 and 1 <= second_digit <= 5:
            print("MASTERCARD")
        elif first_digit == 4:
            print("VISA")
        else:
            print("INVALID")