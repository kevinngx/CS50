# Simulate a sports tournament

import csv
import sys
import random
import math

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    FILENAME = sys.argv[1]

    teams = []
    # TODO: Read teams into memory from file
    with open(FILENAME, newline='') as f:
        reader = csv.reader(f)
        list_reader = list(reader)

    for item in list_reader:
        if item[0] != 'team':
            teams.append({"team": item[0], "rating": int(item[1])})

    # print(teams)
    simulate_tournament(teams)

    counts = {}

    for team in teams:
        counts[team.get("team")] = 0
    print(counts)

    # TODO: Simulate N tournaments and keep track of win counts
    for i in range(0, 1000):
        winner = simulate_tournament(teams)
        counts[winner] = counts[winner] + 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""

    # We use the formula x^n = y --> n = log(y) / log(x)
    rounds = math.log(len(teams))/math.log(2)
    winners = teams

    # print("Number of rounds rounds = " + str(rounds))

    for i in range(0, int(rounds)):
        winners = simulate_round(winners)

    print("Winner is : " + winners[0].get("team"))
    return winners[0].get("team")


if __name__ == "__main__":
    main()
