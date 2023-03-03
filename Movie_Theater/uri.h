.#pragma once
#include "CONSTANTS.h"
#include "AVLHandler.h"
#include "Structs.h"
#include "LLLHandler.h"



void FindMoviesByDay(us day)
{
	int dayOffset = SUNDAY;
	while (dayOffset < NUM_OF_DAYS_IN_WEEK && )
	{
		printf("%d", );
	}

}




void ShowMoviesByHour(ScreeningPtr firstScreenPtr , ScreeningPtr lastScreenPtr ,us delta)
{
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

void printROW();

//הצגת מושבים להקרנה
void ShowMovieSeats(ScreeningPtr screenPtr) 
{
	//Will change SeatsNum later
	short seatsNum;

	//If we add
	//us maxInRow = screenPtr->seats;

	short offset=0;
	short seatsAmount = (seatsNum - 1) / (sizeof(char) + 1);
	while (offset <= seatsAmount) 
	{
		*Mask seat = screenPtr->seats[offset];
		printf("%d\n", (short)seat);
	}
}

//בחירת מושבים בהקרנה
us PickMovieSeats(ScreeningPtr screenPtr, us chosenSeat)
{
	//Will change SeatsNum later
	short seatsNum;

	//If we add
	//us maxInRow = screenPtr->seats;
	ShowMovieSeats(screenPtr);
	screenPtr->seats[chosenSeat] = CHAIR_TAKEN;
}

void ShowMovieTicket(ScreeningPtr screenPtr, us chosenSeat) 
{
	String movie = screenPtr->movie->name;
	printf("%s\n", movie);

	us startTime = screenPtr->hour;
	printf("%hu\n", startTime);

	screenPtr->seatsLeft--;
//  need to add Row length and Col Length
//	us seatRow = screenPtr->seats[offset]/ROWLength;
//	us seatCol = screenPtr->seats[offset] /COLLength;

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

