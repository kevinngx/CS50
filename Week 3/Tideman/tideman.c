#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(int x, int y);
bool check_loop(int base_winner, int loser);
int find_winner(int current_winner);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        for (int n = i + 1; n < candidate_count; n++)
        {
            preferences[ranks[i]][ranks[n]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        //   0 1 2 3
        // 0 [][][][] <-- first iteration
        // 1 [][][][] <-- next iteration starts here, but skips '0'
        // 2 [][][][]
        // 3 [][][][]
        for (int n = 0; n < candidate_count; n++)
        {
            // first check will be [][] vs [][]
            if (i != n && preferences[i][n] > preferences[n][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = n;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Normal sort but instead of raw value check preferences[winner][loser]
    int min_index;
    int strengths[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        strengths[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    // One by one move boundary of unsorted subarray
    // for (int i = 0; i < pair_count; i++)
    // {
    //     // Find the minimum element in unsorted array
    //     // starts at [0][1][2][3][4] i = 0
    //     min_index = i;
    //     for (int n = i + 1; n <= pair_count; n++)
    //     // First chekcs [0][1][2][3][4] n = 1
    //     if (preferences[pairs[n].winner][pairs[n].loser] < preferences[pairs[min_index].winner][pairs[min_index].loser])
    //         min_index = n;
    //     // Swap the found minimum element with the first element
    //     swap(min_index, i);
    // }

    for (int i = 0; i < pair_count; i++)
    {
        min_index = i;
        for (int n = i + 1; n <= pair_count; n++)
        {
            // First chekcs [0][1][2][3][4] n = 1
            if (strengths[n] > strengths[min_index])
            {
                min_index = n;
            }
        }

        // Swap the found minimum element with the first element
        swap(min_index, i);
    }
    return;
}

void swap(int x, int y)
{
    pair tmp = pairs[x];
    pairs[x] = pairs[y];
    pairs[y] = tmp;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int lock_count = 0;
    for (int i = 0; i < pair_count; i++)
    {
        // Check for loops --> trace the loser
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if (check_loop(winner, loser) == true)
        {
            // Add pair to lock
            locked[pairs[i].winner][pairs[i].loser] = true;
            lock_count++;
        }
    }
    return;
}

bool check_loop(int base_winner, int loser)
{
    // [][][]
    // [][][]
    // [][][]
    for (int i = 0; i < candidate_count; i++)
    {
        // Detect a loop if we discover the 'base winner' in the loop
        if (locked[loser][i] == true && i == base_winner)
        {
            return false;
        }

        // If we discover a new connection, delve into that connection
        if (locked[loser][i] == true)
        {
            if (check_loop(base_winner, i) == false)
            {
                return false;
            }
        }
    }
    return true;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int winner = find_winner(0);
    printf("%s\n", candidates[winner]);
    return;
}

int find_winner(int current_winner)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][current_winner] == true)
        {
            return find_winner(i);
        }
    }
    return current_winner;
}
