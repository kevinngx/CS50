import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py sequence database")

    database_file = sys.argv[1]
    sequence_file = sys.argv[2]

    # TODO: Read database file into a variable
    database = []
    database_header = []

    with open(database_file) as file:
        database_reader = csv.reader(file)
        database_header = next(database_reader)
        for row in database_reader:
            database.append(row)
    # print(database_header)
    # print(database)

    # TODO: Read DNA sequence file into a variable
    with open(sequence_file) as file:
        sequence = file.read()
    # print(sequence)

    # TODO: Find longest match of each STR in DNA sequence
    dna_analyzer = {}
    for str in database_header:
        if str != "name":
            dna_analyzer[str] = longest_match(sequence, str)
    # print(dna_analyzer)

    # TODO: Check database for matching profiles
    match = False
    for person in database:
        # print("Checking " + person[0])
        person_dna = {}
        for i in range(1, len(database_header)):
            # print(database_header[i] + " = " + person[i])
            person_dna[database_header[i]] = int(person[i])
        if person_dna == dna_analyzer:
            print(person[0])
            return
        # print(person_dna)

    print("No Match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
