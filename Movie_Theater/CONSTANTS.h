// MACROS

// Booleans
#define TRUE 1
#define FALSE 0

// Project related
#define NUM_OF_DAYS_IN_WEEK 7
#define NUM_OF_CHARS 26
#define NUM_OF_THEATERS 5
#define SCREENING_HOURS_PER_DAY 8
#define CHAIR_TAKEN 1
#define CHAIR_UNOCCUPIED 0

// Number related
#define INIT_VALUE 0
#define ZERO 0
#define ONE 1
#define TWO 2
#define EIGHT 8

// Chars related
#define LOW_A 'a';

// Functions
#define MAX(X,Y) X > Y ? X : Y
#define MIN(X,Y) X < Y ? X : Y
#define GET_MASK_LENGTH(seatsNum) (seatsNum - 1) / sizeof(char) + 1 

// Statistics related
#define CONF_LEVEL 0.95
#define T_SCORE_6_CONF 2.447

#define MAX_STRING_LEN 50