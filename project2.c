/* ENGGEN131 C Project 2023 - Battleships */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 6
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
******* ************************************************************************
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
    int count = 0;

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

/*************************************************************************
** You should define your own test functions below
*************************************************************************/

// Example: a test function for Task One
void TestPrintArray(void)
{
	int map1[MAP_SIZE][MAP_SIZE] = {0};
	printf("Map 1: \n");
	PrintArray(map1);

	int map2[MAP_SIZE][MAP_SIZE] = {0};
	for (int i = 0; i < MAP_SIZE; i++) {
		map2[MAP_SIZE-i-1][i] = i;
	}
	printf("\nMap 2:\n");
	PrintArray(map2);
}

// Example: a test function for Task Two
void TestInitialiseMap(void)
{
int map[MAP_SIZE][MAP_SIZE];
InitialiseMap(map);

	PrintArray(map);
}

// Example: a test function for Task Three
void TestAddRandomShip(void)
{
	int map1[MAP_SIZE][MAP_SIZE] = {0};
	int map2[MAP_SIZE][MAP_SIZE] = {0};

	//AddRandomShip(2, map1);

	//printf("\nMap: \n");
	//PrintArray(map1);

	AddRandomShip(2, map2);
	AddRandomShip(3, map2);
	AddRandomShip(4, map2);
	AddRandomShip(5, map2);

	printf("\nMap: \n");
	PrintArray(map2);
}

void TestCountValues(void)
{
	// int map[MAP_SIZE][MAP_SIZE] = {0};
	// int count, shipSize;

	// InitialiseMap(map);
	// printf("\nMap: \n");	
	// PrintArray(map);
	
	// for (shipSize = 2; shipSize <= 5; shipSize++) {
	// 	count = CountValues(shipSize, map);
	// 	printf("The value %d appears %d times\n", shipSize, count);
	// }
}


void TestTopLeftPosition(void)
{
	// int map[MAP_SIZE][MAP_SIZE] = {0};
	// int row, col, direction, shipSize;
	
	// InitialiseMap(map);
	// PrintArray(map);

	// for (shipSize = 2; shipSize <= 5; shipSize++) {
	// 	direction = TopLeftPosition(shipSize, &row, &col, map);
	// 	printf("Ship %d is at (%d, %d) facing %d\n", shipSize, row, col, direction);
	// }

	// int row, col, direction;
	// int map[MAP_SIZE][MAP_SIZE] = {
  	// {0, 0, 0, 3, 3, 3, 0},
	// {0, 5, 0, 0, 0, 0, 0},
	// {0, 5, 0, 0, 0, 0, 4},
	// {0, 5, 0, 0, 0, 0, 4},
	// {0, 5, 0, 0, 0, 0, 4},
	// {0, 5, 0, 0, 0, 0, 4},
	// {0, 0, 2, 2, 0, 0, 0}
	// };
	// for (int shipSize = 2; shipSize <= 5; shipSize++) {
	// 	direction = TopLeftPosition(shipSize, &row, &col, map);
	// 	printf("Ship %d is at (%d, %d) facing %d\n", shipSize, row, col, direction);
	// }
}

void TestIsShipValid(void)
{
	// int map[MAP_SIZE][MAP_SIZE] = {0};
	// int valid, shipSize;

	// InitialiseMap(map);
	// PrintArray(map);
	// for (shipSize = 2; shipSize <= 5; shipSize++) {
	// 	valid = IsShipValid(shipSize, map);
	// 	printf("Is ship %d valid? %d\n", shipSize, valid);
	// }

	// // Move Ship 3 to an invalid position
	// //map[2][3] = 0;
	// //map[2][0] = 3;

	// // Move Ship 4 to an invalid position
	// //map[6][0] = 0;
	// //map[6][1] = 0;
	// //map[6][2] = 0;
	// //map[6][3] = 0;
	// //map[4][3] = 4;
	// //map[4][4] = 4;
	// //map[4][5] = 4;
	// //map[4][6] = 4;

	// PrintArray(map);
	// for (shipSize = 2; shipSize <= 5; shipSize++) {

	// 	valid = IsShipValid(shipSize, map);
// 	// 	printf("Is ship %d valid? %d\n", shipSize, valid);

// // FOR THIS TEST, MAP_SIZE WILL BE 6


// int map[MAP_SIZE][MAP_SIZE];
// int valid, shipSize;
// int output = 0;

// // Initialise a random (and valid) map 1000 times
// for (int i = 0; i < 1000; i++) {
//    InitialiseRandomMap(map);
//    // All ships are in valid positions
//    // Check that IsShipValid() returns true
//    for (shipSize = 2; shipSize <= 5; shipSize++) {
//       valid = IsShipValid(shipSize, map);
//       if (!valid) {
//          if (!output) {
//             printf("Reporting an invalid ship!\n");
//             PrintArray(map);
//             output = 1;
//          }
//       }
//    }
// }
// printf("All ships are valid");

	// }
// 	int map[MAP_SIZE][MAP_SIZE];
// int valid, shipSize;
// int output = 0;

// // Initialise a random (and valid) map 1000 times
// for (int i = 0; i < 1000; i++) {
//    InitialiseRandomMap(map);
//    // All ships are in valid positions
//    // Check that IsShipValid() returns true
//    for (shipSize = 2; shipSize <= 5; shipSize++) {
//       valid = IsShipValid(shipSize, map);
//       if (!valid) {
//          if (!output) {
//             printf("Reporting an invalid ship!\n");
//             PrintArray(map);
//             output = 1;
//          }
//       }
//    }
// }
// printf("All ships are valid");

	// int valid, shipSize;
	// int map[7][7] = {
	// {0, 0, 0, 0, 0, 0, 4},
	// {0, 0, 0, 0, 0, 0, 4},
	// {0, 0, 0, 0, 0, 0, 4},
	// {0, 0, 0, 2, 0, 0, 4},
	// {0, 0, 0, 2, 0, 0, 0},
	// {5, 5, 5, 3, 5, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0}
	// };
	// for (shipSize = 2; shipSize <= 5; shipSize++) {
	// valid = IsShipValid(shipSize, map);
	// printf("Is ship %d valid? %d\n", shipSize, valid);
	//}


// int map[16][16] = {
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
//     {0, 0, 4, 4, 4, 4, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
//     {0, 2, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0},
//     {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
// };

// int valid, shipSize;
// int output = 0;

// // Initialise a random and valid map 10 times

//    // All ships are in valid positions
//    // Check that IsShipValid() returns true
//    for (shipSize = 2; shipSize <= 5; shipSize++) {
//       valid = IsShipValid(shipSize, map);
//       if (!valid) {
//          if (!output) {
// 			printf("%d", shipSize);
//             printf("Reporting an invalid ship!\n");
//             PrintArray(map);
//             output = 1;
//          }
//       }

// }
// printf("All ships are valid");
}

void TestInitialiseRandomMap(void)
{
		int map[MAP_SIZE][MAP_SIZE] = {0};
		InitialiseRandomMap(map);
		PrintArray(map);
}

void TestFireShot(void)
{
	// int map[MAP_SIZE][MAP_SIZE] = {0};
	// int shots[MAP_SIZE][MAP_SIZE] = {0};

	// InitialiseMap(map);
	// printf("Map:\n");
	// PrintArray(map);
	// printf("Shots:\n");
	// PrintArray(shots);
	// FireShot(shots, map, 2, 0);
	// printf("Shots:\n");
	// PrintArray(shots);
	// FireShot(shots, map, 2, 1);
	// printf("Shots:\n");
	// PrintArray(shots);
	// FireShot(shots, map, 2, 2);
	// printf("Shots:\n");
	// PrintArray(shots);
	// FireShot(shots, map, 2, 3);
	// printf("Shots:\n");
	// PrintArray(shots);
	// FireShot(shots, map, 0, 0);
	// FireShot(shots, map, 1, 0);
	// FireShot(shots, map, 2, 0);
	// FireShot(shots, map, 3, 0);
	// FireShot(shots, map, 4, 0);
	// FireShot(shots, map, 5, 0);
	// FireShot(shots, map, 3, 0);
	// printf("Shots:\n");
	// PrintArray(shots);
// 	int map[MAP_SIZE][MAP_SIZE] = {
// {3, 0, 0, 0, 0, 5},
// {3, 0, 2, 2, 0, 5},
// {3, 0, 0, 0, 0, 5},
// {0, 0, 0, 0, 0, 5},
// {0, 0, 0, 0, 0, 5},
// {4, 4, 4, 4, 0, 0}
// };

// int shots[MAP_SIZE][MAP_SIZE] = {0};
// for (int i = 0; i < MAP_SIZE-1; i++) {
//   for (int j = 0; j < MAP_SIZE; j++) {
//      FireShot(shots, map, i, j);
//   }
// }

// PrintArray(shots);


	int map[MAP_SIZE][MAP_SIZE] = {
	{3, 0, 0, 0, 0, 5},
	{3, 0, 2, 2, 0, 5},
	{3, 0, 0, 0, 0, 5},
	{0, 0, 0, 0, 0, 5},
	{0, 0, 0, 0, 0, 5},
	{4, 4, 4, 4, 0, 0}
	};

	int shots[MAP_SIZE][MAP_SIZE] = {0};
	for (int i = 0; i < MAP_SIZE-1; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			FireShot(shots, map, i, j);
		}
	}
	FireShot(shots, map, 2, 2);

	PrintArray(shots);
}

void TestCheckGameOver(void)
{
	// int map[MAP_SIZE][MAP_SIZE] = {0};
	// int shots[MAP_SIZE][MAP_SIZE] = {0};

	// InitialiseMap(map);
	// printf("Map:\n");
	// PrintArray(map);
	// printf("Shots:\n");
	// PrintArray(shots);

	// while (!CheckGameOver(shots, map)) {
	// 	FireShot(shots, map, rand()%MAP_SIZE, rand()%MAP_SIZE);
// 	// }
// 	// PrintArray(shots);

// 	int map[MAP_SIZE][MAP_SIZE] = {
//   {0, 4, 4, 4, 4, 0, 6},
//   {5, 0, 0, 0, 0, 0, 6},
//   {5, 0, 0, 0, 0, 0, 6},
//   {5, 0, 0, 0, 0, 0, 6},
//   {5, 0, 3, 3, 3, 0, 6},
//   {5, 0, 0, 0, 0, 0, 6},
//   {0, 0, 2, 2, 0, 0, 0}
// };
// int shots[MAP_SIZE][MAP_SIZE] = {
//   {36, 4033, 4029, 4009, 4043, 19, 1013},
//   {5023, 8, 11, 5, 25, 12, 1002},
//   {5017, 10, 26, 41, 47, 40, 1035},
//   {5031, 44, 1, 6, 30, 27, 1021},
//   {5020, 3, 3028, 3022, 3038, 45, 0},
//   {5015, 46, 24, 18, 32, 34, 1014},
//   {4, 0, 2037, 2007, 39, 42, 16}
// };
// printf("Game over? %d\n", CheckGameOver(shots, map));
}

int main(void)
{
	// Initialise the seed for the random number generator
	srand((unsigned int)time(NULL));

	//TestPrintArray();
	//TestInitialiseMap();
	
	//TestAddRandomShip();
	//TestCountValues();
	//TestTopLeftPosition();
	//TestIsShipValid();
	//TestInitialiseRandomMap();
	TestFireShot();
	//TestCheckGameOver();

	return 0;
}