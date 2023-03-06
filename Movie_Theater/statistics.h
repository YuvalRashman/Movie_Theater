#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "searches.h"

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

// **** Extract summery of week / day ****

// O(n) - אורך הקלט הוא מספר האיברים במטריצה
void InitSummery(int** summeryPtrStart, int** summeryPtrEnd, int moviesNum)
{
	int** iterPtr = summeryPtrStart;

	// Loop on all elements
	while (iterPtr <= summeryPtrEnd)
	{
		// Calloc eanough space for all movies + one for sum
		*iterPtr = (int*)calloc(moviesNum + ONE, sizeof(int));

		// Next
		iterPtr++;
	}
}

// O(1) - אין אורך קלט
void CaclSumOfDay(MovieTheaterPtr* theaters, DaySchedulePtr dayScreenings, int** daySummery, int moviesNum)
{
	ScreeningPtr* startPtr = *dayScreenings->screeningsSchedule;
	ScreeningPtr* iterPtr = startPtr;
	ScreeningPtr* endPtr = iterPtr + NUM_OF_THEATERS * SCREENING_HOURS_PER_DAY - ONE;
	int currHour;
	int seatsTaken;
	int currMovie;

	// Loop on all elemtns in the matrix (fixed size)
	while (iterPtr <= endPtr)
	{
		// Find current hour (column)
		currHour = (iterPtr - startPtr) % SCREENING_HOURS_PER_DAY;

		// If there is a screening and it begins in the current hour
		if (*iterPtr && (*iterPtr)->hour == currHour)
		{
			// Find the seats sold in the current screening
			seatsTaken = theaters[(*iterPtr)->theaterId]->totalSeats - (*iterPtr)->seatsLeft;
			currMovie = (*iterPtr)->movie->movieId - ONE;

			// Update the sum in the summery
			*(*(daySummery + currHour) + currMovie) += seatsTaken;

			// Update sum of all sums of movies per hour
			*(*(daySummery + currHour) + moviesNum) += seatsTaken;

			// Update sum of all sums of hours per movie
			*(*(daySummery + SCREENING_HOURS_PER_DAY) + currMovie) += seatsTaken;

			// Update the sum of the day
			*(*(daySummery + SCREENING_HOURS_PER_DAY) + moviesNum) += seatsTaken;
		}

		// Next
		iterPtr++;
	}
}

// O(1) - אין אורך קלט
DaySummery MakeDaySummery(MovieTheaterPtr* theaters, DaySchedulePtr dayScreenings, int moviesNum) {
	DaySummery daySummery;
	
	// Init the new summery of the day
	InitSummery(daySummery.summeryMatrix, daySummery.summeryMatrix + SCREENING_HOURS_PER_DAY, moviesNum + ONE);

	// Calculate the summery
	CaclSumOfDay(theaters, dayScreenings, daySummery.summeryMatrix, moviesNum);

	return daySummery;
}

// O(1) - אין אורך קלט
WeekSummery MakeWeekSummery(MovieTheaterPtr* theaters, WeekSchedulePtr weekStats, int moviesNum) {
	WeekSummery weekSummery;
	us currOffset;
	DaySchedulePtr currDaySchedule;

	// Loop on week's screenings history
	for (currOffset = ZERO; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		// For each day of the week create a new day summery!
		currDaySchedule = weekStats->weekSchedule[currOffset];
		weekSummery.weekSummery[currOffset] = MakeDaySummery(theaters, currDaySchedule, moviesNum);
	}

	return weekSummery;
}

// **** conclusions on the summery ****

// O(1) - המעבר בלי קשר לאורך הקלט
us FindBussiestDay(WeekSummeryPtr weekSummeryPtr, int moviesNum) {
	us bussiestDay = INIT_VALUE;
	int currOffset;
	int temp;

	// Init max to the first day
	int max = weekSummeryPtr->weekSummery[INIT_VALUE].summeryMatrix[SCREENING_HOURS_PER_DAY][moviesNum];

	// For each day from the second one:
	for (currOffset = ONE; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		temp = weekSummeryPtr->weekSummery[currOffset].summeryMatrix[SCREENING_HOURS_PER_DAY][moviesNum];
		bussiestDay = temp > max ? currOffset : bussiestDay;
	}

	return bussiestDay;
}

// O(1) - המעבר בלי קשר לאורך הקלט
us FindBussiestHour(WeekSummeryPtr weekSummeryPtr, us moviesNum) {
	us hourOffset;
	us dayOffset;

	// Init a vector to hold the sum of each hour of all days
	int hoursSum[SCREENING_HOURS_PER_DAY] = { INIT_VALUE };

	// For each day
	for (dayOffset = INIT_VALUE; dayOffset < NUM_OF_DAYS_IN_WEEK; dayOffset++)
	{
		// For each hour
		for (hourOffset = INIT_VALUE; hourOffset < SCREENING_HOURS_PER_DAY; hourOffset++)
		{
			// Add sum
			hoursSum[hourOffset] += weekSummeryPtr->weekSummery[dayOffset].summeryMatrix[hourOffset][moviesNum];
		}
	}

	// Find max day in the vector
	return FindMaxOffset(hoursSum, hoursSum + SCREENING_HOURS_PER_DAY - ONE, ONE);
}

// O(n) - אורך הקלט מייצג את מספר הסרטים הקיימים
us FindMostWantedMovie(WeekSummeryPtr weekSummeryPtr, int moviesNum) {
	us movieOffset;
	us dayOffset;
	us mostWantedMovie;

	// Create a vector to hold the sum of each movie in the week
	int* moviesSum = (int*)calloc(moviesNum, sizeof(int));

	// For each day
	for (dayOffset = INIT_VALUE; dayOffset < NUM_OF_DAYS_IN_WEEK; dayOffset++)
	{
		// For each movie
		for (movieOffset = INIT_VALUE; movieOffset < moviesNum; movieOffset++)
		{
			// Add sum
			moviesSum[movieOffset] += weekSummeryPtr->weekSummery[dayOffset].summeryMatrix[SCREENING_HOURS_PER_DAY][movieOffset];
		}
	}

	mostWantedMovie = FindMaxOffset(moviesSum, moviesSum + moviesNum - ONE, ONE);
	free(moviesSum);
	return mostWantedMovie;
}

// O(1) - אורך הקלט קבוע - מספר הימים
void ExpectationsPerDay(WeekSummeryPtr weekSummeryPtr, int moviesNum, double* lowerBound, double* upperBound) {
	int currOffset;
	int temp;
	double mean = INIT_VALUE, std = INIT_VALUE, E, lower, upper;

	// Calc mean and STD
	for (currOffset = INIT_VALUE; currOffset < NUM_OF_DAYS_IN_WEEK; currOffset++)
	{
		// Sum of the day
		temp = weekSummeryPtr->weekSummery[currOffset].summeryMatrix[SCREENING_HOURS_PER_DAY][moviesNum];

		// Mean and std
		mean += temp;
		std += pow(temp, TWO);
	}

	// Find min
	mean /= NUM_OF_DAYS_IN_WEEK;

	// Find s^2
	std = std / NUM_OF_DAYS_IN_WEEK - pow(mean, TWO);

	// Find S^
	std = sqrt(std * NUM_OF_DAYS_IN_WEEK / (NUM_OF_DAYS_IN_WEEK - ONE));

	// Calculate the margin of error
	E = T_SCORE_6_CONF * std / sqrt(NUM_OF_DAYS_IN_WEEK);

	// Calculate the lower and upper bounds of the confidence interval
	*lowerBound = mean - E;
	*upperBound = mean + E;
}