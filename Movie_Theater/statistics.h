-#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "AVLHandler.h"
#include "CONSTANTS.h"
#include "LLLHandler.h"
#include "structs.h"

MovieTheaterPtr theaters[NUM_OF_THEATERS];

// **** Usefull methods ****

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

// **** Extract summery of week / day ****

// O(n)
void InitSummery(int** summeryPtrStart, int** summeryPtrEnd, int moviesNum)
{
	int** iterPtr = summeryPtrStart;

	while (iterPtr <= summeryPtrEnd)
	{
		*iterPtr = (int*)calloc(moviesNum, sizeof(int));
		iterPtr++;
	}
}

// O(n)
void CaclSumOfDay(DaySchedulePtr dayScreenings, int** daySummery, int moviesNum)
{
	ScreeningPtr* iterPtr = *dayScreenings->screeningsSchedule;
	ScreeningPtr* endPtr = iterPtr + NUM_OF_THEATERS * SCREENING_HOURS_PER_DAY - ONE;
	int currHour;
	int seatsTaken;
	int currMovie;

	while (iterPtr <= endPtr)
	{
		// Find the seats sold in the current screening
		seatsTaken = theaters[(*iterPtr)->theaterId]->ColNum * theaters[(*iterPtr)->theaterId]->rowNum - (*iterPtr)->seatsLeft;

		// Find current hour
		currHour = iterPtr % SCREENING_HOURS_PER_DAY;
		currMovie = (*iterPtr)->movie->MovieId;

		if ((*iterPtr)->hour == currHour)
		{
			// Update the sum
			*(*(daySummery + currHour) + currMovie) += seatsTaken;

			// Update sum of all sums of movies per hour
			*(*(daySummery + currHour) + moviesNum) += seatsTaken;

			// Update sum of all sums of hours per movie
			*(*(daySummery + SCREENING_HOURS_PER_DAY) + currMovie) += seatsTaken;

			// Update the sum of the day
			*(*(daySummery + SCREENING_HOURS_PER_DAY) + moviesNum) += seatsTaken;
		}

		iterPtr++;
	}
}

// O(n)
void MakeDaySummery(DaySchedulePtr dayScreenings, int moviesNum) {
	DaySummery daySummery;
	InitSummery(daySummery.summeryMatrix, daySummery.summeryMatrix + SCREENING_HOURS_PER_DAY, moviesNum + ONE);

	CaclSumOfDay(dayScreenings, daySummery.summeryMatrix, moviesNum);

	return daySummery;
}

// O(n)
WeekSummery MakeWeekSummery(WeekSchedulePtr weekStats, int moviesNum) {
	WeekSummery weekSummery;
	us currOffset;
	DaySchedulePtr currDaySchedule;

	// Loop on week's screenings history
	for (currOffset = ZERO; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		currDaySchedule = weekStats->weekSchedule[currOffset];
		weekSummery.weekSummery[currOffset] = MakeDaySummery(currDaySchedule, moviesNum);
	}

	return weekSummery;
}

// **** conclusions on the summery ****

// O(1)
DAY FindBussiestDay(WeekSummeryPtr weekSummeryPtr, int moviesNum) {
	DAY bussiestDay = SUNDAY;
	int max = weekSummeryPtr->weekSummery[INIT_VALUE].summeryMatrix[SCREENING_HOURS_PER_DAY][moviesNum];
	int currOffset;

	int temp;


	for (currOffset = ONE; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		temp = weekSummeryPtr->weekSummery[currOffset].summeryMatrix[SCREENING_HOURS_PER_DAY][moviesNum];
		bussiestDay = temp > max ? currOffset : bussiestDay;
	}

	return bussiestDay;
}

// O(1)
us FindBussiestHour(WeekSummeryPtr weekSummeryPtr) {
	us hourOffset;
	us dayOffset;
	int hoursSum[SCREENING_HOURS_PER_DAY] = { INIT_VALUE };

	for (dayOffset = INIT_VALUE; dayOffset < NUM_OF_DAYS_IN_WEEK; dayOffset++)
	{
		for (hourOffset = INIT_VALUE; hourOffset < SCREENING_HOURS_PER_DAY; hourOffset++)
		{
			hoursSum[hourOffset] += weekSummeryPtr->weekSummery[dayOffset].summeryMatrix[SCREENING_HOURS_PER_DAY][hourOffset];
		}
	}

	return FindMaxOffset(hoursSum, hoursSum + SCREENING_HOURS_PER_DAY - ONE, ONE);
}

// O(n)
void FindMostWantedMovie(WeekSummeryPtr weekSummeryPtr, int moviesNum) {
	us movieOffset;
	us dayOffset;
	int* moviesSum = (int*)calloc(moviesNum, sizeof(int));

	for (dayOffset = INIT_VALUE; dayOffset < NUM_OF_DAYS_IN_WEEK; dayOffset++)
	{
		for (movieOffset = INIT_VALUE; movieOffset < moviesNum; movieOffset++)
		{
			moviesSum[movieOffset] += weekSummeryPtr->weekSummery[dayOffset].summeryMatrix[movieOffset][moviesNum];
		}
	}

	return FindMaxOffset(moviesSum, moviesSum + moviesNum - ONE, ONE);
}

// O(1)
void ExpectationsPerDay(WeekSummeryPtr weekSummeryPtr, int moviesNum, double* lowerBound, double* upperBound) {
	int currOffset;
	int temp;
	double mean = INIT_VALUE, std = INIT_VALUE, tScore, E, lower, upper;

	// Calc mean and STD
	for (currOffset = INIT_VALUE; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{

		temp = weekSummeryPtr->weekSummery[currOffset].summeryMatrix[SCREENING_HOURS_PER_DAY][moviesNum];
		mean += temp;
		std += pow(temp - mean, TWO);
	}

	mean /= NUM_OF_DAYS_IN_WEEK;
	std /= (NUM_OF_DAYS_IN_WEEK - ONE);
	std = sqrt(std);

	// Look up the t-score from a t-table based on the level of confidence and the sample size
	tScore = 2.447; // t (0.95, 6)

	// Calculate the margin of error
	E = tScore * std / sqrt(NUM_OF_DAYS_IN_WEEK);

	// Calculate the lower and upper bounds of the confidence interval
	*lowerBound = mean - E;
	*upperBound = mean + E;
}