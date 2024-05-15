/* ENGGEN131 C Project 2023 - Battleships */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 10
#define HIT 1000

// Prototype declarations of required functions
void PrintArray(int values[MAP_SIZE][MAP_SIZE]);
void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]);
void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]);
int CountValues(int value, int map[MAP_SIZE][MAP_SIZE]);
int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE]);
int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]);
void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]);
void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col);
int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]);


/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go below
*******************************************************************************
*******************************************************************************
******************************************************************************/

void PrintArray(int values[MAP_SIZE][MAP_SIZE])
{
	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){
			printf("%d ", values[i][j]);
		}
		printf("\n");
	}
}

void InitialiseMap(int map[MAP_SIZE][MAP_SIZE])
{

	for(int i = 0; i < MAP_SIZE; i++){
		for(int j = 0; j< MAP_SIZE; j++){
			map[i][j] = 0;
		}
	}
	
	// Placing the 2 long boat 
	for (int i = 0; i < 2; i++){
		map[i][0] = 2;
	}

	// Placing the 3 long boat 
	for(int i = 0; i < 3; i++){
		map[2][i+1] = 3;
	}

	// Placing the 4 long boat
	for(int i = 0; i < 4; i++){
		map[MAP_SIZE-1][i] = 4;
	}

	// Placing the 5 long boat
	for (int i = 0; i < 5; i++){
		map[MAP_SIZE - 5 + i][MAP_SIZE - 1] = 5;
	}

}

void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE])
{
	int horzorvert, positioni, positionj;

	// Choose horizontal or vertical 
	horzorvert = rand() % 2;

	//If 0 then horizontal
	if (horzorvert == 0){
		positioni = rand() % (MAP_SIZE);
		positionj = rand() % (MAP_SIZE - size + 1);

		for (int j = positionj; j < (size + positionj); j++){
			map[positioni][j] = size;
		}

	} else {
		positioni = rand() % (MAP_SIZE - size + 1);
		positionj = rand() % (MAP_SIZE);

		for (int i = positioni; i < (size + positioni); i++){
			map[i][positionj] = size;
		}
	}

}

int CountValues(int value, int map[MAP_SIZE][MAP_SIZE])
{
	int count = 0;

	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){

			if (value == map[i][j]){
				count++;
			}
		}
	}
	return count;
}

int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE]) {

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (size == map[i][j]) {
                *row = i;
                *col = j;
                if (j + 1 < MAP_SIZE && map[i][j + 1] == size) {
                    return 1;
                } else if (i + 1 < MAP_SIZE && map[i + 1][j] == size) {
                    return 2;
                }
            }
        }
    }
    return 0;
}

int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]) {

    int row, col, direction, count;
    direction = TopLeftPosition(size, &row, &col, map);

    if (direction == 1) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= size; j++) {
                if (row + i >= 0 && row + i < MAP_SIZE && col + j >= 0 && col + j < MAP_SIZE && 
					(i != -1 || j != -1) && (i != -1 || j != size) && (i != 1 || j != -1) && (i != 1 || j != size)) {
                    if ((map[row + i][col + j] != 0) && (map[row + i][col + j] != size)){
                        return 0;
                    }
                }
            }
        }
    } else {
        for (int i = -1; i <= size; i++) {
            for (int j = -1; j <= 1; j++) {
                if (row + i >= 0 && row + i < MAP_SIZE && col + j >= 0 && col + j < MAP_SIZE && 
					(i != -1 || j != -1) && (i != size || j != -1) && (i != -1 || j != 1) && (i != size || j != 1)) {
                    if ((map[row + i][col + j] != 0) && (map[row + i][col + j] != size)) {
                        return 0;
                    }
                }
            }
        }
    }

	// Is it overlapping so if the value doesnt equal the number of times the boat is counted
	count = CountValues(size, map);

	if (count != size){
		return 0;
	}

    return 1;
}


void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE])
{
    int valid;
    int countloop = 0, count = 0;

    while (count != 4)
    {
        // Add random ship
        for (int i = 2; i <= 5; i++)
        {
            AddRandomShip(i, map);
        }

        // Check if each ship is valid
		for (int shipSize = 2; shipSize <= 5; shipSize++) 
		{
			valid = IsShipValid(shipSize, map);
			if (valid == 1){
				count = count + 1;
			}
		}
		if (count != 4) 
		{
			count = 0;
			for (int i = 0; i < MAP_SIZE; i++) {
            	for (int j = 0; j < MAP_SIZE; j++) {
                map[i][j] = 0;
            	}
			}
		} 

    }
}


void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col)
{
	int currentcount = 1;

	// Make sure its not a spot we have already hit 
	if (shots[row][col] == 0) {

		// Check through the array to find the last iteration of this function
		for(int i = 0; i< MAP_SIZE; i++){
				for(int j = 0; j< MAP_SIZE; j++){
					if (shots[i][j] != 0){
						currentcount ++; 
					}
				}
			}

		// Check the fired shot hits a boat in the the map array
		if (map[row][col] != 0){
			shots[row][col] = HIT + currentcount;
		} else {
			shots[row][col] = currentcount;
			return;
		}

		// Checking if any of the boats are destroyed 
            int shipSize = map[row][col];
            int isShipDestroyed = 1;
            for (int i = 0; i < MAP_SIZE; i++) {
                for (int j = 0; j < MAP_SIZE; j++) {
                    if (map[i][j] == shipSize && shots[i][j] == 0) {
                        isShipDestroyed = 0;
                        return;
                    }
                }
			}

            if (isShipDestroyed) {
                for (int i = 0; i < MAP_SIZE; i++) {
                    for (int j = 0; j < MAP_SIZE; j++) {
                        if (map[i][j] == shipSize) {
                            shots[i][j] = shots[i][j] - HIT + shipSize * HIT;
                        }
                    }
                }
            }
        } 
}

int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]) {
    // Check through the map array to find if the position in the ship does not equal zero,
    // if it does, see if it has been hit; if it hasn't been destroyed, it returns a 0 meaning the game ain't over.
    // Then, at the end, it returns a 1 if it doesn't return a 0.
    for(int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] != 0) {
                if (shots[i][j] < 2000)
                    return 0;
            }
        }
    }
    return 1;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go above
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Provided functions needed for playing the Battleships game:

// First, prototype declarations for the bots:
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col);
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col);

// Gets the input for one move from the human player (an alphabetic row and a numeric column)
// This function converts both inputs to numeric values
void GetMoveHuman(int *row, int *col, int player)
{
	char a = ' ';
	int b = -1;
	printf("Player %d: enter move [row/col]: ", player);
	while (!(a >= 'A' && a <= 'Z')) {
		scanf("%c", &a);
	}
	while (!(b >= 0 && b <= 25)) {
		scanf("%d", &b);
	}
	*row = (int)(a - 'A');
	*col = b;
}

// Takes two "shots" arrays as input (which are the visible parts of the game shown to players) and formats them into a string for printing
// The player currently to move is highlighted
void GetDisplayMapString(int shots1[MAP_SIZE][MAP_SIZE], int shots2[MAP_SIZE][MAP_SIZE], int player, char *boardString)
{
	int i, j;
	char numbers[10];
	// Intialise board string to be empty:
	boardString[0] = '\0';
	strcat(boardString, "  ");

	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i%10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i%10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "\n  ");

	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "\n");

	for (i = 0; i < MAP_SIZE; i++) {
		int len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len+1] = '\0';
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots1[i][j] / HIT > 0) {
				if (shots1[i][j] / HIT == 1) {
					strcat(boardString, "X");
				} else {
					sprintf(numbers, "%d", shots1[i][j] / HIT);
					strcat(boardString, numbers);
				}
			} else if (shots1[i][j] > 0) {
				strcat(boardString, ".");
			} else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots2[i][j] / HIT > 0) {
				if (shots2[i][j] / HIT == 1) {
					strcat(boardString, "X");
				} else {
					sprintf(numbers, "%d", shots2[i][j] / HIT);
					strcat(boardString, numbers);
				}
			} else if (shots2[i][j] > 0) {
				strcat(boardString, ".");
			} else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len+1] = '\0';
		strcat(boardString, "\n");
	}
	if (player == 1) {
		strcat(boardString, "  P1");
		for (i = 0; i < MAP_SIZE-2; i++) {
			strcat(boardString, "*");
		}
	} else {
		for (i = 0; i < MAP_SIZE; i++) {
			strcat(boardString, " ");
		}
		strcat(boardString, "   P2");
		for (i = 0; i < MAP_SIZE-2; i++) {
			strcat(boardString, "*");
		}
	}
	strcat(boardString, "\n");
}

// Plays one game of Battleships, beginning with the specified starting player
// Game type = 1 (human vs human) or 2 (human vs bot) or 3 (bot vs bot)
int PlayOneGame(int startingPlayer, int gameType)
{
	int row, col, player, gameOver;

	// String to display the boards
	char displayBoardString[(2*MAP_SIZE+5)*(MAP_SIZE+5)];

	// The maps containing the locations of the ships
	int mapPlayer1[MAP_SIZE][MAP_SIZE] = {0};
	int mapPlayer2[MAP_SIZE][MAP_SIZE] = {0};

	// The locations of the shots
	int shotsPlayer1[MAP_SIZE][MAP_SIZE] = {0};
	int shotsPlayer2[MAP_SIZE][MAP_SIZE] = {0};

	player = startingPlayer;
	gameOver = 0;

	// Create random maps for each player
	InitialiseRandomMap(mapPlayer1);
	InitialiseRandomMap(mapPlayer2);

	// Display the board if a human is playing
	if (gameType != 3) {
		GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
		printf("%s", displayBoardString);
	}

	// Process one move for the current player
	while (!gameOver) {
		if (gameType == 1) {
			GetMoveHuman(&row, &col, player);
			if (player == 1) {
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		} else if (gameType == 2) {
			if (player == 1) {
				GetMoveHuman(&row, &col, player);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				GetMoveBot1(shotsPlayer2, &row, &col);
				printf("Player 2 (bot) moves: %c%d\n", (char)(row+'A'), col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		} else {
			if (player == 1) {
				GetMoveBot1(shotsPlayer1, &row, &col);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				GetMoveBot2(shotsPlayer2, &row, &col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}

		// Swap players
		if (!gameOver) {
			player = 3 - player;
		}
		if (gameType != 3) {
			GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
			printf("%s", displayBoardString);
		}
	}
	return player;
}

// Play a Battleships tournament
// If the tournament is between a human and a bot, only one game is played
// If the tournament is between two bots, the number of games is requested
void PlayBattleships(void)
{
	int gameType, numberOfGames, result;
	int i, wins1, wins2, player;

	// Get play options:
	printf("Options:\n");
	printf(" [1] = Human vs. Human\n");
	printf(" [2] = Human vs. Bot1\n");
	printf(" [3] = Bot1 vs. Bot2\n");
	printf("Choose game type: ");
	scanf("%d", &gameType);
	numberOfGames = 1;
	result = 0;

	// If two bots are playing a tournament, let the user choose how many games
	if (gameType == 3) {
		printf("Number of games: ");
		scanf("%d", &numberOfGames);
	}

	// Set win counts to zero
	wins1 = 0;
	wins2 = 0;

	// Player 1 will always start the first game
	// If a tournament is played (between two bots), the starting player alternates
	player = 1;

	for (i = 0; i < numberOfGames; i++) {
		result = PlayOneGame(player, gameType);
		if (result == 1) {
			wins1++;
		} else {
			wins2++;
		}
		// Switch the starting player for the next game
		player = 3 - player;
	}

	// Show the outcome (of a single game or tournament)
	if (numberOfGames == 1) {
		printf("\nGame over! Congratulations! Winner is Player %d\n\n", result);
	} else {
		printf("\nTournament over! Games played = %d\nPlayer 1 wins = %d / Player 2 wins = %d\n\n", numberOfGames, wins1, wins2);
	}
}

// The main function for the Battleships program
int main(void)
{
	// Initialise the seed for the random number generator
	srand((unsigned int)time(NULL));

	printf("ENGGEN131 - C Project 2023\n");
	printf("                          ... presents ...\n");
	printf(" ___    __   _____ _____  _     ____  __   _     _   ___   __  \n");
	printf("| |_)  / /\\   | |   | |  | |   | |_  ( (` | |_| | | | |_) ( (` \n");
	printf("|_|_) /_/--\\  |_|   |_|  |_|__ |_|__ _)_) |_| | |_| |_|   _)_) \n");
	printf("\n                __/___            \n");
	printf("          _____/______|           \n");
	printf("  _______/_____\\_______\\_____     \n");
	printf("  \\              < < <       |    \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n");

	PlayBattleships();

	return 0;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the two bots can be provided below
// The existing implementations are naive:
// - Bot 1 simply takes random shots (without checking for repeated shots)
// - Bot 2 also takes random shots (but makes a single test on each turn for a repeated shot)
*******************************************************************************
*******************************************************************************
******************************************************************************/

void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col)
{
    int hitrow = -1;
    int hitcol = -1;
    int isthereashotbyitself = 1;
    int rand_row = rand() % MAP_SIZE;
    int rand_col = rand() % MAP_SIZE;

    // 1st step check if there is a hit already but not destroyed
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if ((hitrow == -1 && hitcol == -1) && (shots[i][j] > 1000 && shots[i][j] < 2000))
            {
                hitrow = i;
                hitcol = j;
            }
        }
    }

    // 2nd step if there is a hit already then check if it is by itself
    if (hitrow != -1 && hitcol != -1)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (hitrow + i >= 0 && hitrow + i < MAP_SIZE && hitcol + j >= 0 && hitcol + j < MAP_SIZE && (i != 0 || j != 0))
                {
                    // Checking if it's been hit
                    if (shots[hitrow + i][hitcol + j] > 1000)
                    {
                        isthereashotbyitself = 0;
                    }
                }
            }
        }
    }

    // 3rd step if it is then check around the space we have hit but not destroyed and
    // if there is no other hit around that hit we continue to hit around that spot until we hit a ship.
    // Do NOT hit the spaces that have already been hit first hit to the left up right and then below and make
    // sure when we fire it's inside the bounds
    if (isthereashotbyitself == 1)
    {
        if ((hitcol - 1 >= 0) && (shots[hitrow][hitcol - 1] == 0))
        {
            *row = hitrow;
            *col = hitcol - 1;
            return;
        }
        else if ((hitrow - 1 >= 0) && (shots[hitrow - 1][hitcol] == 0))
        {
            *row = hitrow - 1;
            *col = hitcol;
            return;
        }
        else if ((hitcol + 1 < MAP_SIZE) && (shots[hitrow][hitcol + 1] == 0))
        {
            *row = hitrow;
            *col = hitcol + 1;
            return;
        }
        else if ((hitrow + 1 < MAP_SIZE) && (shots[hitrow + 1][hitcol] == 0))
        {
            *row = hitrow + 1;
            *col = hitcol;
            return;
        }
    }

    // 4th step else if we find that there are other hits next to the place we hit but haven't destroyed
    // then we find if it's vertical or horizontal and if it's vertical we hit at the top or bottom of the hits
    // and if it's horizontal we hit to the left or right of the two hits, do not hit if it's already been hit.

    // 5th step if there are no hits found then randomly pick a place on the board until we hit a ship and do not
    // hit a place we have hit before.
    while (shots[rand_row][rand_col] != 0)
    {
        rand_row = rand() % MAP_SIZE;
        rand_col = rand() % MAP_SIZE;
    }

    *row = rand_row;
    *col = rand_col;
}


// Strategy for Bot 2
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col)
{
    static int last_hit_row = -1;
    static int last_hit_col = -1;

    // Define a grid to hold probabilities for each cell
    double probabilities[MAP_SIZE][MAP_SIZE] = {0};

    // Step 1: Update probabilities based on previous hits
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (shots[i][j] > 1000 && shots[i][j] < 2000) {
                if (i > 0 && shots[i - 1][j] == 0) probabilities[i - 1][j] += 0.25;
                if (i < MAP_SIZE - 1 && shots[i + 1][j] == 0) probabilities[i + 1][j] += 0.25;
                if (j > 0 && shots[i][j - 1] == 0) probabilities[i][j - 1] += 0.25;
                if (j < MAP_SIZE - 1 && shots[i][j + 1] == 0) probabilities[i][j + 1] += 0.25;
            }
        }
    }

    // Step 2: Find the cell with the highest probability
    double max_prob = -1;
    *row = -1;
    *col = -1;

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (shots[i][j] == 0 && probabilities[i][j] > max_prob) {
                max_prob = probabilities[i][j];
                *row = i;
                *col = j;
            }
        }
    }

    // Step 3: If no hits yet, or no ships are partially hit, make a random move
    if (*row == -1 || *col == -1) {
        int rand_row = rand() % MAP_SIZE;
        int rand_col = rand() % MAP_SIZE;

        while (shots[rand_row][rand_col] != 0) {
            rand_row = rand() % MAP_SIZE;
            rand_col = rand() % MAP_SIZE;
        }

        *row = rand_row;
        *col = rand_col;
    }
    // Step 4: If there are partially hit ships, continue targeting them
    else {
        last_hit_row = *row;
        last_hit_col = *col;
    }
}