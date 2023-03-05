#define _CRT_SECURE_NO_WARNINGS
#include "AVLHandler.h"

MovieTheaterPtr theaters[NUM_OF_THEATERS];
WeekSchedulePtr weekSchedule;
MovieHandlerPtr movieHandler;
us moviesNum;

// Hash for movies
us hash(MoviePtr movie)
{
	return *movie->name - 'a';
}

MoviePtr FindMovie(String movieName, us movieId)
{
	return (MoviePtr)Search(movieHandler->movieLists[hash(movieName)], movieId)->info;
}
us SumVec(us* vecStartPtr, us* vecEndPtr, us delta)
{
	us* iterPtr = vecStartPtr;
	us sum = INIT_VALUE;

	while (iterPtr <= vecEndPtr)
	{
		sum += *iterPtr;
		iterPtr += delta;
	}

	return sum;
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
MovieTheaterPtr NewTheater(us id)
{
	MovieTheaterPtr newTheater = (MovieTheaterPtr)malloc(sizeof(struct MovieTheater));

	printf("New theater:\n");

	// rowNum
	printf("Enter row numbers: ");
	scanf("%hd", &newTheater->rowNum);

	// colNum
	printf("Enter column numbers: ");
	scanf("%hd", &newTheater->colNum);

	// theaterId
	newTheater->theaterId = id;

	// totalSeats
	newTheater->totalSeats = SumVec(newTheater->rowsSeats, newTheater->rowsSeats + newTheater->rowNum - ONE, ONE);

	// rowsSeats
	printf("Enter seats on each row: ");
	newTheater->rowsSeats = NewArray(newTheater->rowNum);

	return newTheater;
}
void InitTheaters()
{
	us counter;

	for (counter = INIT_VALUE; counter < NUM_OF_THEATERS; counter++)
	{
		theaters[counter] = NewTheater(counter);
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
ScreeningPtr InputNewScreening(us* screeningDay)
{
	us movieId;
	String movieName;
	ScreeningPtr newScreening = (ScreeningPtr)malloc(sizeof(Screening));

	// movieId
	printf("Please enter movie id: ");
	scanf("%hd", &movieId);

	// movieName
	printf("Please enter movie name: ");
	scanf("%s", &movieName);

	// movie
	newScreening->movie = FindMovie(movieName, movieId);

	// theaterId
	printf("Please input theater id: ");
	scanf("%hd", &newScreening->theaterId);

	// hour
	printf("Please input an hour: ");
	scanf("%hd", &newScreening->hour);

	// day
	printf("Please enter the day of the screening (number): ");
	scanf("%d", screeningDay);

	// seatsLeft
	newScreening->seatsLeft = theaters[newScreening->theaterId]->totalSeats;

	// seats
	us numOfMasks = (theaters[newScreening->theaterId]->totalSeats - ONE) / sizeof(Mask) + ONE;
	newScreening->seats = (Mask*)calloc(numOfMasks, sizeof(Mask));

	return newScreening;
}
void InsertScreeningToMatrix(ScreeningPtr* startingPlace, ScreeningPtr newScreening)
{
	ScreeningPtr* endPlace = startingPlace + (int)fmin((int)newScreening->movie->length, SCREENING_HOURS_PER_DAY - newScreening->hour - ONE);
	ScreeningPtr* iterPtr = startingPlace;

	while (iterPtr <= endPlace)
	{
		*iterPtr = newScreening;
		iterPtr++;
	}
}
void InputScreenings()
{
	char inpMessage;
	ScreeningPtr newScreening;
	us day;
	MoviePtr currMovie;
	ScreeningPtr* startingPlace;

	printf("Would you like to input a new screening? (y/n) ");
	scanf("%c", &inpMessage);

	while (inpMessage == 'y')
	{
		newScreening = InputNewScreening(&day);

		// Put screening in matrix
		startingPlace = weekSchedule->weekSchedule[day]->screeningsSchedule[newScreening->theaterId] + newScreening->hour;
		InsertScreeningToMatrix(startingPlace, newScreening);

		// Put screening in movie's tree
		currMovie = FindMovie(newScreening->movie->name, newScreening->movie->movieId);
		currMovie->days[day] = Insert(currMovie->days[day], (void*)newScreening, newScreening->hour);


		printf("Would you like to input a new screening? (y/n) ");
		scanf("%c", inpMessage);
	}
}

void Init() {
	InitTheaters(); // Completed
	InitNewWeekSchedule(); // Completed
	InitMovieHandler(); // Completed
	InputMovies(); // Completed
	InputScreenings(); // completed
}

// Free structure
void FreeMatrix(DaySchedulePtr matrix)
{
	ScreeningPtr* iterPtr = *matrix->screeningsSchedule;
	ScreeningPtr* endPtr = iterPtr + NUM_OF_THEATERS * SCREENING_HOURS_PER_DAY - ONE;

	while (iterPtr <= endPtr)
	{
		free((*iterPtr)->seats);
		free(*iterPtr);
		iterPtr++;
	}
}
void FreeTree(Node* root)
{
	if (!root)
		return;

	FreeTree(root->left);
	FreeTree(root->right);
	free(root);
}
void FreeMovieTree(Node* movieTree)
{
	us counter;

	if (!movieTree)
		return;

	// PostOrder
	FreeMovieTree(movieTree->left);
	FreeMovieTree(movieTree->right);

	// Free day-trees
	for (counter = INIT_VALUE; counter < NUM_OF_DAYS_IN_WEEK; counter++)
	{
		FreeTree(((MoviePtr)movieTree->info)->days[counter]);
	}

	// Free into and node
	free(movieTree->info);
	free(movieTree);
}
void FreeAll()
{
	us counter;

	// Free theaters
	for (counter = INIT_VALUE; counter < NUM_OF_THEATERS; counter++)
	{
		free(theaters[counter]->rowsSeats);
		free(theaters[counter]);
	}

	// Free weekSchedule
	for (counter = INIT_VALUE; counter < NUM_OF_DAYS_IN_WEEK; counter++)
	{

		FreeMatrix(weekSchedule->weekSchedule[counter]);
		free(weekSchedule->weekSchedule[counter]);
	}
	free(weekSchedule);

	// Free movieHandler
	for (counter = INIT_VALUE; counter < NUM_OF_CHARS; counter++)
	{
		FreeMovieTree(movieHandler->movieLists[counter]);
	}
	free(movieHandler);
}

int main()
{
	Init();
	FreeAll();
}