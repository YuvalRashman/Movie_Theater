#define _CRT_SECURE_NO_WARNINGS
#include "AVLHandler.h"

MovieTheaterPtr theaters[NUM_OF_THEATERS];
WeekSchedulePtr weekSchedule;
MovieHandlerPtr movieHandler;
us moviesNum;

MoviePtr FindMovie(String movieName, us movieId)
{
	return (MoviePtr)Search(movieHandler->movieLists[hash(movieName)], movieId)->info;
}

// Hash for movies
int hash(MoviePtr movie)
{
	return *movie->name - 'a';
}

// Theaters
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

// Movie Handler
void InitMovieHandler()
{
	// calloc - usefull for allocating memory for all lists as manager = NULL
	movieHandler = (MovieHandlerPtr)calloc(ONE, sizeof(MovieHandler));
}

// Week Schedule
void InitNewWeekSchedule()
{
	us counter;
	weekSchedule = (WeekSchedulePtr)malloc(sizeof(WeekSchedule));

	// Allocate memory for all daySchedules
	for (counter = INIT_VALUE; counter < NUM_OF_DAYS_IN_WEEK; counter++)
	{
		// Calloc - for initializing all screenings in the matrix as NULL!
		weekSchedule->weekSchedule[counter] = (DaySchedulePtr)calloc(ONE, sizeof(DaySchedule));
	}
}

// New Movies
MoviePtr InputNewMovie(us movieId)
{
	MoviePtr newMovie = (MoviePtr)calloc(ONE, sizeof(Movie));

	printf("New movie:\n");

	// movieId
	newMovie->movieId = movieId;

	// name
	printf("Enter movie name: ");
	scanf("%s", newMovie->name);

	// length
	printf("Enter movie length (in hours): ");
	scanf("%f", &newMovie->length);

	return newMovie;
}
void InputMovies()
{
	us counter;
	MoviePtr movieInp;

	// Scan for the number of movies
	printf("Please input the initial number of movies: ");
	scanf("%hd", &moviesNum);

	for (counter = INIT_VALUE; counter < moviesNum; counter++)
	{
		movieInp = InputNewMovie(counter);
		movieHandler->movieLists[hash(movieInp)] = Insert(movieHandler->movieLists[hash(movieInp)], (void*)movieInp, movieInp->movieId);
	}
}

// New Screenings
void InputNewScreening()
{
	// use of FindMovie()!
}
void InputScreenings()
{

}

void Init() {
	InitTheaters(); // Completed
	InitNewWeekSchedule(); // Completed
	InitMovieHandler(); // Completed
	InputMovies(); // Completed
	InputScreenings(); // To be completed - input screenings and arrange it in the matrixes and trees
}

int main()
{
	Init();
}