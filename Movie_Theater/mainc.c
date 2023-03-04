#include "AVLHandler.h"

MovieTheaterPtr theaters[NUM_OF_THEATERS];
WeekSchedulePtr weekSchedule;
MovieHandlerPtr movieHandler;

us* NewArray(us len)
{
	us* vector = (us*)malloc(sizeof(us) * len);
	us counter;

	for (counter = INIT_VALUE; counter < len; counter++)
	{
		scanf("%hd", vector + counter);
	}

	return vector;
}
MovieTheaterPtr NewTheater()
{
	MovieTheaterPtr newTheater = (MovieTheaterPtr)malloc(sizeof(struct MovieTheater));

	printf("New theater:\n");

	// rowNum
	printf("Enter row numbers: ");
	scanf("%hd", &newTheater->rowNum);

	// colNum
	printf("Enter column numbers: ");
	scanf("%hd", &newTheater->ColNum);

	// theaterId
	printf("Enter theater id: ");
	scanf("%hd", &newTheater->theaterId);

	// totalSeats
	printf("Enter total seats: ");
	scanf("%hd", &newTheater->totalSeats);

	// rowsSeats
	printf("Enter seats on each row: ");
	newTheater->rowsSeats = NewArray(newTheater->rowNum);

	return newTheater;
}
void InitTheaters()
{
	us counter;

	for (counter = INIT_VALUE; counter <= NUM_OF_THEATERS; counter++)
	{
		theaters[counter] = NewTheater();
	}
}

void Init() {
	InitTheaters(); // Completed
	InitNewWeekSchedule(); // To be completed
	InitMovieHandler(); // To be completed
}

int main()
{

}