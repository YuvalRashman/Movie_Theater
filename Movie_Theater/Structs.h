// structs file

#include <stdlib.h>
#include <stdio.h>
#include "CONSTANTS.h"
#include "LLLHandler.h"

// String
typedef char* String;

// Mask
typedef char Mask;

// us
typedef unsigned short us;

// AVL Node
typedef struct node {
	void* info;
	us key;
	us height;
	struct node* left;
	struct node* right;
} Node, *NodePtr;

// Movie
typedef struct Movie {
	us movieId;
	String name;
	float length;
	Node* days[NUM_OF_DAYS_IN_WEEK];
} Movie, *MoviePtr;

// MoviesHandler
typedef struct MovieHandler {
	Node* movieLists[NUM_OF_CHARS];
} MovieHandler, *MovieHandlerPtr;

// MovieTheater
typedef struct MovieTheater {
	us rowNum;
	us colNum;
	us theaterId;
	us* rowsSeats; //diynamic array
	us totalSeats;
} MovieTheater, *MovieTheaterPtr;

// Screenings
typedef struct Screening {
	MoviePtr movie;
	us theaterId;
	us hour;
	Mask* seats;
	us seatsLeft;
} *ScreeningPtr, Screening;

// Week and Day - schedule
typedef struct DaySchedule {
	ScreeningPtr screeningsSchedule[NUM_OF_THEATERS][SCREENING_HOURS_PER_DAY];
} *DaySchedulePtr, DaySchedule;

typedef struct WeekSchedule {
	DaySchedulePtr weekSchedule[NUM_OF_DAYS_IN_WEEK];
} *WeekSchedulePtr, WeekSchedule;

// Enum
typedef	enum DaysOfWeek {
	SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
};

// Summeries
typedef struct DaySummery {
	int* summeryMatrix[SCREENING_HOURS_PER_DAY + ONE];
} *DaySummaryPtr, DaySummery;

typedef struct WeekSummery {
	DaySummery weekSummery[NUM_OF_DAYS_IN_WEEK];
} *WeekSummeryPtr, WeekSummery;