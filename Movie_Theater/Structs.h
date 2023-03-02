// structs file

#include <stdlib.h>
#include <stdio.h>
#include <C:\Users\nadavru\Documents\GitHub\Movie_Theater\Movie_Theater\AVLHandler.h>
#include <C:\Users\nadavru\Documents\GitHub\Movie_Theater\Movie_Theater\CONSTANTS.h>

// String
typedef char* String;

typedef char Mask;
typedef unsigned short us;

// Movie
typedef struct Moive {
	String name;
	float length;
	Node* days[NUM_OF_DAYS_IN_WEEK];
} *MoviePtr;

// MovieTheater
typedef struct MovieTheater {
	us rowNum;
	us ColNum;
	us theaterId;
} *MovieTheaterPtr;

// Screenings
typedef struct Screening {
	us movieId;
	us theaterId;
	us hour;
	Mask* seats;
	us seatsLeft;
} *ScreeningPtr;

// Week and Day - schedule
typedef struct DaySchedule {
	ScreeningPtr screeningsSchedule[SCREENING_HOURS_PER_DAY][NUM_OF_THEATERS];
} *DaySchedulePtr;

typedef struct WeekSchedule {
	DaySchedule weekSchedule[NUM_OF_DAYS_IN_WEEK];
} *WeekSchedulePtr;


