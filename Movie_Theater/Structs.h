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
typedef struct Movie {
	us MovieId;
	String name;
	float length;
	Node* days[NUM_OF_DAYS_IN_WEEK];
} *MoviePtr;

// MoviesHanler
typedef struct MoveHandler {
	LLLManager movieLists[NUM_OF_CHARS];
} *MoveHandlerPtr;

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
	ScreeningPtr screeningsSchedule[SCREENING_HOURS_PER_DAY][NUM_OF_THEATERS];
} *DaySchedulePtr;

typedef struct WeekSchedule {
	DaySchedule weekSchedule[NUM_OF_DAYS_IN_WEEK];
} *WeekSchedulePtr;


