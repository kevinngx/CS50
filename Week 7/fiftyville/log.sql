-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Theft took place July 28th
-- Theft took place at Chamberlin Street

-- The THIEF is:
-- The city the thief ESCAPED TO:
-- The ACCOMPLICE is:

-- Query 1 - find police reports
SELECT
FROM crime_scene_reports
WHERE month = 7 AND day = 28;

-- Crime ID = 295
-- Location = Humphrey Street
-- Description Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- Query 2 - find three interviews on the day of the crime which mention the bakery
SELECT *
FROM interviews
WHERE month = 7 AND day = 28
AND transcript like '%bakery%';

-- Witnesses / Suspects
-- Ruth: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond: As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Leads
-- L01 Check security footage from bakery parking lot --> within 10 mins of the theft (crime took place 10:15am)
-- L02 Thief withdrew money from ATM on Leggett street earlier in the day (before 10:15am)
-- L03 Thief flying out of fiftyville July 29, accomplice purchased the flight ticket

-- Query 3 - Check L01: Security Cam Footage, getting license plates for cars exiting the location
select license_plate from bakery_security_logs
where month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and activity = 'exit';

-- Query 4 - Check L02: ATM Withdrawel
select account_number from atm_transactions
where month = 7 and day = 28 and transaction_type = 'withdraw' and atm_location = 'Leggett Street';

-- Query 5 - Follow up L02, find names attached to accounts
select * from bank_accounts ba
inner join people p on ba.person_id = p.id
where ba.account_number in
(
    select account_number from atm_transactions
    where month = 7 and day = 28 and transaction_type = 'withdraw' and atm_location = 'Leggett Street'
)

-- Query 6 - Check licence plates from L01 and L02
select p.name, p.phone_number, p.passport_number, p.license_plate from bank_accounts ba
inner join people p on ba.person_id = p.id
where ba.account_number in
(
    -- Query 5
    select account_number from atm_transactions
    where month = 7 and day = 28 and transaction_type = 'withdraw' and atm_location = 'Leggett Street'
)
and p.license_plate in
(
    -- Query 3
    select license_plate from bakery_security_logs
    where month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and activity = 'exit'
)
;

-- Query 7 - L03: Call Logs
select * from phone_calls
where month = 7 and day = 28 and duration <= 60;

-- Query 8 - Trace phone call
select p.name, p.phone_number, p.passport_number, p.license_plate from bank_accounts ba
inner join people p on ba.person_id = p.id
where ba.account_number in
(
    -- Query 5
    select account_number from atm_transactions
    where month = 7 and day = 28 and transaction_type = 'withdraw' and atm_location = 'Leggett Street'
)
and p.license_plate in
(
    -- Query 3
    select license_plate from bakery_security_logs
    where month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and activity = 'exit'
)
and p.phone_number in
(
    select caller from phone_calls
    where month = 7 and day = 28 and duration <= 60
)
;

-- Suspect is Bruce or Diana

-- Query 9 - L03: Outbound flight (earliest flight)
select * from flights
where month = 7 and day = 29
order by hour, minute
limit 1;

-- Get list of passengers flying out on the 29th
select * from passengers p
inner join flights f on p.flight_id = f.id
where f.id in
(
    select id from flights
    where month = 7 and day = 29
);

-- Query 10 Find which suspect is flying out tomorrow

select p.name, p.phone_number, p.passport_number, p.license_plate from bank_accounts ba
inner join people p on ba.person_id = p.id
where ba.account_number in
(
    -- Query 5 -- finds people who withdrew money that day
    select account_number from atm_transactions
    where month = 7 and day = 28 and transaction_type = 'withdraw' and atm_location = 'Leggett Street'
)
and p.license_plate in
(
    -- Query 3 -- finds people who left the scene within 10 mins of the robbery
    select license_plate from bakery_security_logs
    where month = 7 and day = 28 and hour = 10 and minute >= 15 and minute <= 25 and activity = 'exit'
)
and p.phone_number in
(
    -- Finds people who made a call on that day
    select caller from phone_calls
    where month = 7 and day = 28 and duration <= 60
)
and passport_number in
(
    -- Person has a ticket for a flight the next day
    select p.passport_number from passengers p
    inner join flights f on p.flight_id = f.id
    where f.id in
    (
        select id from flights
        where month = 7 and day = 29
        order by hour, minute
        limit 1
    )
);

-- Suspect is Bruce

-- Query 7 - L03: Call Logs
select * from phone_calls
where month = 7 and day = 28 and duration <= 60 and caller = '(367) 555-5533';

-- Accomplice = (375) 555-8161


select * from people
where phone_number = '(375) 555-8161';