#pragma once
#include "CONSTANTS.h"
#include "AVLHandler.h"
#include "Structs.h"
#include "LLLHandler.h"


//Show All Screenings Per Hour
LLLManager GetClosestScreeningsByHour(WeekSchedulePtr weekSchedule, us day)
{
	// create an array of screenings
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	ScreeningPtr* iterPtr = daySchedule->screeningsSchedule[INIT_VALUE][INIT_VALUE];
	ScreeningPtr* endPtr = daySchedule->screeningsSchedule[NUM_OF_THEATERS][SCREENING_HOURS_PER_DAY];
	LLLManager managerLLL;

	InitLLL(managerLLL);

	while (iterPtr <= endPtr)
	{
		if ((*iterPtr)->hour == hour)
			PushLLL(&managerLLL, (void*)*iterPtr);

		iterPtr += SCREENING_HOURS_PER_DAY;
	}
	return managerLLL;
}

//Show All Screenings Per Day
LLLManager GetClosestScreeningsByDay(WeekSchedulePtr weekSchedule, us day)
{
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	ScreeningPtr* iterPtr = daySchedule->screeningsSchedule[INIT_VALUE][INIT_VALUE];
	ScreeningPtr* endPtr = daySchedule->screeningsSchedule[NUM_OF_THEATERS][SCREENING_HOURS_PER_DAY];
	LLLManager managerLLL;

	InitLLL(managerLLL);

	while (iterPtr <= endPtr)
	{
		if ((*iterPtr)->hour == hour)
			PushLLL(&managerLLL, GetClosestScreeningsByDay(weekSchedule, day));

		iterPtr++;;
	}
	return managerLLL;
}










void ShowMoviesByDay(WeekSchedule )
{
	int dayOffset = SUNDAY;
	while (dayOffset < NUM_OF_DAYS_IN_WEEK&&)
	{
		ShowMovieByHour;
	}

}

void ShowMoviesByHour(DaySchedule daySchedule )
{
	ScreeningPtr lastScreenPtr, us delta
	ScreeningPtr firstScreenPtr;
	String movie;
	float length;

	while (firstScreenPtr <= lastScreenPtr)
	{
		movie = *firstScreenPtr->movie;
		printf("%s\n", movie);

		length = *firstScreenPtr->hour;
		printf("%d\n", length);

		firstScreenPtr += delta;
	}
}
void ShowMovieByHour(ScreeningPtr firstScreenPtr, ScreeningPtr lastScreenPtr, us delta)
{
	String movie;
	float length;

	while (firstScreenPtr <= lastScreenPtr)
	{
		if (firstScreenPtr != screenPtr++ || firstScreenPtr == lastScreenPtr)
		{
			movie = *firstScreenPtr->movie;
			printf("%s\n", movie);

			hour = *firstScreenPtr->hour;
			printf("%d\n", length);

		}
		firstScreenPtr += delta;
	}
}

void ShowMoviesByDay(DaysOfWeek day)
{
	ScreeningPtr firstScreenPtr = WeekSchedulePtr[day];
	ScreeningPtr lastScreenPtr = firstScreenPtr + SCREENING_HOURS_PER_DAY-1;
	ScreeningPtr lastTheaterPtr = firstScreenPtr + (SCREENING_HOURS_PER_DAY-1 * NUM_OF_THEATERS);
	us delta;
	while (firstScreenPtr <= lastScreenPtr && firstScreenPtr <= lastTheaterPtr)
	{
		ShowMoviesByHour(firstScreenPtr, lastTheaterPtr,delta)
		firstScreenPtr += delta;
	}
}


void ShowMovieByDay(int movieID, DaysOfWeek day)
{
	ScreeningPtr firstScreenPtr = WeekSchedulePtr[day];
	ScreeningPtr lastScreenPtr = screenPtr + NUM_OF_THEATERS;
	us delta;
	
	while ((firstScreenPtr < lastScreenPtr))
	{
		ShowMovieByHour(firstScreenPtr, lastScreenPtr, delta);
	    screenPtr += delta;
	}

}

//Movie Seats
//äöâú îåùáéí ìä÷øðä
void ShowMovieSeats(DaySchedulePtr dayPtr ,ScreeningPtr screenPtr) 
{
	us seatsInRow;
	char bit;
	us totalSeats = 0;
    int row;
	for (us rowOffset = 0; rowOffset < theaters[screenPtr->teatherID]->rowNum; rowOffset++)
	{
		seatsInRow = theaters[screenPtr->teatherID]->rowSeats[rowOffset];
		for (us seat = 0; seat < seatsInRow; seat++)
		{
			bit = (screenPtr->seats[totalSeats / 8] & (1 << totalSeats % 8)) >> totalSeats % 8;
			printf("%c", bit);
			
				totalSeats++;
		}
		printf("\n");
	}
}

//áçéøú îåùáéí áä÷øðä
void PickMovieSeats(ScreeningPtr screenPtr, us seat)
{
	ShowMovieSeats(screenPtr);
	screenPtr->seats[seat / 8] |= CHAIR_TAKEN << seat % 8;
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
	prinft("%hu %hu", rowNumber, sum - chosenSeat);
}



us TakenSeatsPercent(ScreeningPtr screenPtr)
{
	short seatsAmount = (seatsNum - 1) / (sizeof(char) + 1);
	short takenPercent = (screenPtr->seatsLeft / seatsAmount);
	return takenPercent;
}



ScreeningPtr MaxAnticipatedByHour(ScreeningPtr firstScreenPtr, ScreeningPtr lastScreenPtr, us delta) 
{
	us curr;
	ScreeningPtr maxPtr = firstScreenPtr;
	lastScreenPtr = firstScreenPtr + SCREENING_HOURS_PER_DAY - 1;
	us maxPercent = TakenSeatsPercent(firstScreenPtr);
	while (firstScreenPtr <= lastScreenPtr)
	{

		curr = MovieTakenSeatsPercent(firstScreenPtr);
		MAX(curr, maxPercent) == maxPercent ? maxPtr = firstScreenPtr : NULL;
		maxPercent = MAX(curr, maxPercent);
		firstScreenPtr += delta;
	}
	return maxPtr;
}


ScreeningPtr MaxAnticipatedByDay(ScreeningPtr firstScreenPtr, ScreeningPtr lastScreenPtr, us delta)
{
	ScreeningPtr firstScreenPtr = WeekSchedulePtr[day];
	ScreeningPtr lastScreenPtr = firstScreenPtr + SCREENING_HOURS_PER_DAY - 1;
	ScreeningPtr curr;
	ScreeningPtr maxPtr = firstScreenPtr;
	while (firstScreenPtr < lastScreenPtr)
	{
		curr = MaxAnticipatedByHour(firstScreenPtr, lastScreenPtr, delta);
		maxPtr = TakenSeatsPercent(maxPtr) >= TakenSeatsPercent(curr) ? maxPtr : curr;
		firstScreenPtr += delta;
	}
	return maxPtr;

}

ScreeningPtr MaxAnticipatedByWeek(ScreeningPtr firstScreenPtr, ScreeningPtr lastScreenPtr, us delta)
{
	ScreeningPtr firstScreenPtr = WeekSchedulePtr[day];
	ScreeningPtr lastScreenPtr = firstScreenPtr + NUM_OF_DAYS_IN_WEEK-1;
	ScreeningPtr curr;
	ScreeningPtr maxPtr = firstScreenPtr;
	while (firstScreenPtr < lastScreenPtr)
	{
		curr = MaxAnticipatedByDay(firstScreenPtr, lastScreenPtr, delta);
		maxPtr = TakenSeatsPercent(maxPtr) >= TakenSeatsPercent(curr) ? maxPtr : curr;
		firstScreenPtr += delta;
	}
	return maxPtr;

}

