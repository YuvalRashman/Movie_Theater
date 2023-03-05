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
	us totalSeats = 0;
    int row;
	for (us rowOffset = 0; rowOffset < theaters[screenPtr->theaterId]->rowNum; rowOffset++)
	{
		seatsInRow = theaters[screenPtr->theaterId]->rowSeats[rowOffset];
		for (us seat = 0; seat < seatsInRow; seat++)
		{
			bit = (screenPtr->seats[(totalSeats / 8)] & (1 << totalSeats % 8)) >> (totalSeats % 8);
			printf("%c", bit);
			
				totalSeats++;
		}
		printf("\n");
	}
}

void PickMovieSeats(ScreeningPtr screenPtr, us seat)
{
	ShowMovieSeats(screenPtr);
	screenPtr->seats[(seat / 8)] |= CHAIR_TAKEN << (seat % 8);
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
	for (rowNumber = 0; sum < chosenSeat; sum += theaters[screenPtr->theaterId]->rowSeats[offset], rowNumber++);
	printf("%hu %hu", rowNumber, sum - chosenSeat);
}
