#pragma once
#include "CONSTANTS.h"
#include "AVLHandler.h"
#include "Structs.h"
#include "LLLHandler.h"
#include "math.h"

//Movie Seats
void ShowMovieSeats(ScreeningPtr screenPtr) 
{
	us seatsInRow;
	char bit;
	us totalSeats = ZERO;
    int row;
	for (us rowOffset = ZERO; rowOffset < theaters[screenPtr->theaterId]->rowNum; rowOffset++)
	{
		seatsInRow = theaters[screenPtr->theaterId]->rowSeats[rowOffset];
		for (us seat = ZERO; seat < seatsInRow; seat++)
		{
			bit = (screenPtr->seats[(totalSeats / EIGHT)] & (ONE << totalSeats % EIGHT)) >> (totalSeats % EIGHT);
			printf("%c", bit);
			
				totalSeats++;
		}
		printf("\n");
	}
}

void PickMovieSeats(ScreeningPtr screenPtr, us seat)
{
	ShowMovieSeats(screenPtr);
	screenPtr->seats[(seat / EIGHT)] |= CHAIR_TAKEN << (seat % EIGHT);
	screenPtr->seatsLeft--;
}

void ShowMovieTicket(ScreeningPtr screenPtr, us chosenSeat) 
{
	String movie = screenPtr->movie->name;
	printf("%s\n", movie);

	us startTime = screenPtr->hour;
	printf("%hu\n", startTime);
	us rowNumber;
	us sum;
	for (rowNumber = ZERO; sum < chosenSeat; sum += theaters[screenPtr->theaterId]->rowSeats[rowNumber], rowNumber++);
	printf("%hu %hu", rowNumber, sum - chosenSeat);
}
