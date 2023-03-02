#include <stdlib.h>
#include <stdio.h>
#include "AVLHandler.h"
#include "CONSTANTS.h"
#include "LLLHandler.h"
#include "structs.h"

MovieTheaterPtr theaters[NUM_OF_THEATERS];


// Bussiest hour

// O(n) - אורך הקלט כמות האיברים במערך
int SumVec(ScreeningPtr vecStartPtr, ScreeningPtr vecEndPtr, us delta)
{
	int sum = INIT_VALUE;
	us numOfSeatsInTheater;

	while (vecStartPtr <= vecEndPtr)
	{
		numOfSeatsInTheater = (theaters[vecStartPtr->theaterId]->ColNum * theaters[vecStartPtr->theaterId]->rowNum);
		sum += numOfSeatsInTheater - vecStartPtr->seatsLeft;
		vecStartPtr += delta;
	}

	return sum;
}

// O(n*m) - כמות הטורים mכמות השורות ו n
void AddSumCols(ScreeningPtr** matrix, us rowLen, us colLen, int* vecPtr)
{
	us currCol;

	for (currCol = INIT_VALUE; currCol <= rowLen; currCol++)
	{
		*(vecPtr + currCol) += SumVec(matrix[INIT_VALUE][currCol], matrix[colLen - ONE][currCol], rowLen) / NUM_OF_DAYS_IN_WEEK;
	}
}

// O(n) - אורך הקלט הוא גודל המערך
us FindMaxOffset(int* vecPtr, int* vecEndPtr, us delta)
{
	us maxOffset = INIT_VALUE;
	int* iterPtr = vecPtr + delta;

	while (iterPtr <= vecEndPtr)
	{
		maxOffset = *iterPtr > *(vecPtr + maxOffset) ? iterPtr - vecPtr : maxOffset;
		iterPtr += delta;
	}

	return maxOffset;
}

// O(1) - אין אורך קלט, הפונקציה עוברת על מספר קבוע של מטריצות אשר מורכבות ממימדים באורכים קבועים!
us FindBussiestHour(WeekSchedulePtr weekStats)
{
	us currOffset;
	DaySchedulePtr currDaySchedule;
	int sumHours[NUM_OF_DAYS_IN_WEEK] = { INIT_VALUE };


	for (currOffset = ZERO; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		currDaySchedule = weekStats->weekSchedule[currOffset];
		AddSumCols(currDaySchedule->screeningsSchedule, SCREENING_HOURS_PER_DAY, NUM_OF_THEATERS, sumHours);
	}

	return FindMaxOffset(sumHours, sumHours + NUM_OF_DAYS_IN_WEEK - ONE, ONE);
}


// Bussiest day

// O(1) - אין אורך קלט, הפונקציה עוברת על מספר קבוע של מטריצות אשר מורכבות ממימדים באורכים קבועים!
us FindBussiestDay(WeekSchedulePtr weekStats)
{
	us currOffset;
	DaySchedulePtr currDaySchedule;
	int sumPerDay[NUM_OF_DAYS_IN_WEEK] = { INIT_VALUE };


	for (currOffset = ZERO; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		currDaySchedule = weekStats->weekSchedule[currOffset];
		sumPerDay[currOffset] += SumVec(currDaySchedule->screeningsSchedule[INIT_VALUE][INIT_VALUE], 
										currDaySchedule->screeningsSchedule[NUM_OF_THEATERS - ONE][SCREENING_HOURS_PER_DAY - ONE],
										ONE);
	}

	return FindMaxOffset(sumHours, sumHours + NUM_OF_DAYS_IN_WEEK - ONE, ONE);
}

