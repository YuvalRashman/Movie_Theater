// structs file

#include <stdlib.h>
#include <stdio.h>
#include "AVLHandler.h"
#include "CONSTANTS.h"
#include "LLLHandler.h"

// String
typedef char* String;

typedef char Mask;
typedef unsigned short us;

// Movie
typedef struct Movie {
	us MovieId;
	String name;
	float length;
	Node* days[NUM_OF_DAYS_IN_WEEK];
} *MoviePtr;

// MoviesHanler
typedef struct MovieHandler {
	LLLManager movieLists[NUM_OF_CHARS];
} *MovieHandlerPtr;

// MovieTheater
typedef struct MovieTheater {
	us rowNum;
	us ColNum;
	us theaterId;
} *MovieTheaterPtr;

// Screenings
typedef struct Screening {
	MoviePtr movie;
	us theaterId;
	us hour;
	Mask* seats;
	us seatsLeft;
} *ScreeningPtr;

// Week and Day - schedule
typedef struct DaySchedule {
	ScreeningPtr screeningsSchedule[NUM_OF_THEATERS][SCREENING_HOURS_PER_DAY];
} *DaySchedulePtr;

typedef struct WeekSchedule {
	DaySchedulePtr weekSchedule[NUM_OF_DAYS_IN_WEEK];
} *WeekSchedulePtr;

typedef	enum DaysOfWeek {
	SUNDAY = 1, MONDAY = 2, TUESDAY = 3, WEDNESDAY = 4, THURSDAY = 5, FRIDAY = 6, SATURDAY = 7
};

