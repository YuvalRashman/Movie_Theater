#define _CRT_SECURE_NO_WARNINGS
#include "searches.h"
#include <string.h>

MovieTheaterPtr theaters[NUM_OF_THEATERS];
WeekSchedulePtr weekSchedule;
MovieHandlerPtr movieHandler;
us moviesNum;

// Hash for movies
us hash(String movie)
{
	return *(movie) - 'a';
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
	char tempName[MAX_STRING_LEN];

	printf("New movie:\n");

	// movieId
	newMovie->movieId = movieId;

	// name
	printf("Enter movie name: ");
	scanf("%s", tempName);
	newMovie->name = _strdup(tempName);

	// length
	printf("Enter movie length (in hours): ");
	scanf("%f", &newMovie->length);

	return newMovie;
}

void newDataMovie(int movieId, char* movieName, int length) {
	MoviePtr movie = (MoviePtr)calloc(1, sizeof(Movie));
	movie->length = length;
	movie->movieId = movieId;
	movie->name = movieName;
	int offset = hash(movie->name);
	movieHandler->movieLists[offset] = Insert(movieHandler->movieLists[offset], (void*)movie, movieId);
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
	char movieName[MAX_STRING_LEN];
	ScreeningPtr newScreening = (ScreeningPtr)malloc(sizeof(Screening));

	// movieId
	printf("Please enter movie id: ");
	scanf("%hd", &movieId);

	// movieName
	printf("Please enter movie name: ");
	scanf("%s", movieName);

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

// Searches on movies
LLLManager SearchMovieFromHour(String movieName, us movieId, us day, us hour) {
	MoviePtr currMovie = FindMovie(movieName, movieId);
	Node* root = ClosestHigherKey(currMovie->days[day], hour);
	LLLManager manager = NULL;

	// Make list from all nodes in the current sub-tree
	TreeToListInOrder(&manager, root);
	return manager;
}

LLLManager SearchMovieByHour(String movieName, us movieId, us day, us hour) {
	MoviePtr moviePtr = FindMovie(movieName, movieId);
	Node* screenings = moviePtr->days[day];

	return (LLLNodePtr)ClosestHigherKey(screenings, hour)->info;
}

void newScreening(int day, MoviePtr movie, us theaterId, int hour) {
	ScreeningPtr screening = (ScreeningPtr)calloc(1 ,sizeof(Screening));
	screening->movie = movie;
	screening->seatsLeft = theaters[theaterId]->totalSeats;
	screening->theaterId = theaterId;
	screening->seats = (Mask*) calloc((theaters[theaterId]->totalSeats / sizeof(char)), sizeof(Mask)); // add malloc to mask
	screening->hour = hour; // ??? redandecy
	int hourOffset;
	int endHour = ceil(hour + movie->length / 60);
	for (hourOffset = hour; hourOffset < endHour; hourOffset++)
	{
		weekSchedule->weekSchedule[day]->screeningsSchedule[theaterId][hourOffset] = screening;
	}
	Insert(movie->days[day], (void*)screening, hour);
}

MovieTheaterPtr newDataTheater(us theaterId, us rowNum, us colNum, us* rowsSeats, us totalSeats) {
	MovieTheaterPtr theater = (MovieTheaterPtr) calloc(1, sizeof(MovieTheater));
	theater->colNum = colNum;
	theater->rowNum = rowNum;
	theater->theaterId = theaterId;
	theater->totalSeats = totalSeats;
	theater->rowsSeats = rowsSeats;
	return theater;
}

void InitData() {
	
	InitMovieHandler();
	InitNewWeekSchedule();

	//create movies
	newDataMovie(1, _strdup("ant man and the wasp"), 120);
	newDataMovie(2, _strdup("hulk"), 60);
	newDataMovie(3, _strdup("star wars"), 40);
	newDataMovie(4, _strdup("kill bill"), 20);
	newDataMovie(5, _strdup("twenty one jump street"), 50);
	newDataMovie(6, _strdup("spongebob the movie (best)"), 120);
	newDataMovie(7, _strdup("avengers 1"), 80);
	newDataMovie(8, _strdup("harry poter solving all your problems"), 50);
	newDataMovie(9, _strdup("space jam"), 49);

	//create theaters
	int arr1[4] = { 6, 10, 15, 20 };
	theaters[0] = newDataTheater(0, 4, 20, arr1, 51);
	int arr2[3] = { 4 ,3 ,3 };
	theaters[1] = newDataTheater(1, 3, 10, arr2, 16);
	int arr3[5] = { 8,6,6,6, 12 };
	theaters[2] = newDataTheater(2, 5, 12, arr3, 38);
	int arr4[2] = { 5,5 };
	theaters[3] = newDataTheater(3, 2, 5, arr4, 10);
	int arr5[6] = { 10,10,10,10,10,10 };
	theaters[4] = newDataTheater(4, 6, 10, arr5, 60);

	//create screening
	newScreening(0, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(0, FindMovie("hulk", 2), 1, 1);

	newScreening(0, FindMovie("star wars", 3), 2, 5);
	newScreening(0, FindMovie("star wars", 3), 3, 0);
	newScreening(0, FindMovie("star wars", 3), 4, 2);
	newScreening(0, FindMovie("star wars", 3), 2, 0);
	newScreening(0, FindMovie("star wars", 3), 2, 7);

	newScreening(0, FindMovie("kill bill", 4), 3, 4);
	newScreening(0, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(0, FindMovie("spongebob the movie (best)", 6), 0, 4);

	newScreening(1, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(1, FindMovie("hulk", 2), 1, 1);
	newScreening(1, FindMovie("star wars", 3), 2, 0);
	newScreening(1, FindMovie("kill bill", 4), 3, 4);
	newScreening(1, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(1, FindMovie("spongebob the movie (best)", 6), 0, 4);

	newScreening(2, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(2, FindMovie("hulk", 2), 1, 1);
	newScreening(2, FindMovie("star wars", 3), 2, 0);
	newScreening(2, FindMovie("kill bill", 4), 3, 4);
	newScreening(2, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(2, FindMovie("spongebob the movie (best)", 6), 0, 4);

	newScreening(3, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(3, FindMovie("hulk", 2), 1, 1);
	newScreening(3, FindMovie("star wars", 3), 2, 0);
	newScreening(3, FindMovie("kill bill", 4), 3, 4);
	newScreening(3, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(3, FindMovie("spongebob the movie (best)", 6), 0, 4);

	newScreening(4, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(4, FindMovie("hulk", 2), 1, 1);
	newScreening(4, FindMovie("star wars", 3), 2, 0);
	newScreening(4, FindMovie("kill bill", 4), 3, 4);
	newScreening(4, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(4, FindMovie("spongebob the movie (best)", 6), 0, 4);

	newScreening(5, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(5, FindMovie("hulk", 2), 1, 1);
	newScreening(5, FindMovie("star wars", 3), 2, 0);
	newScreening(5, FindMovie("kill bill", 4), 3, 4);
	newScreening(5, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(5, FindMovie("spongebob the movie (best)", 6), 0, 4);

	newScreening(6, FindMovie("ant man and the wasp", 1), 0, 6);
	newScreening(6, FindMovie("hulk", 2), 1, 1);
	newScreening(6, FindMovie("star wars", 3), 2, 0);
	newScreening(6, FindMovie("kill bill", 4), 3, 4);
	newScreening(6, FindMovie("twenty one jump street", 5), 4, 5);
	newScreening(6, FindMovie("spongebob the movie (best)", 6), 0, 4);
}

int main()
{
	InitData();
	FreeAll();
}