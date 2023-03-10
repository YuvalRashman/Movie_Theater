#define _CRT_SECURE_NO_WARNINGS
#include "statistics.h"
#include <string.h>

// Globals
MovieTheaterPtr theaters[NUM_OF_THEATERS];
WeekSchedulePtr weekSchedule;
MovieHandlerPtr movieHandler;
us moviesNum;

// **** Useful Methods ****

// O(log(n)) - uses open hash and searches in a balanced tree
// אורך הקלט הוא מספר הצמתים בעץ
MoviePtr FindMovie(String movieName, us movieId)
{
	return (MoviePtr) Search(movieHandler->movieLists[hash(movieName)], movieId)->info;
}

// **** Init Methods ****

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
void newDataMovie(int movieId, char* movieName, float length) {
	MoviePtr movie = (MoviePtr)calloc(1, sizeof(Movie));
	movie->length = length;
	movie->movieId = movieId;
	movie->name = movieName;
	int offset = hash(movie->name);
	movieHandler->movieLists[offset] = Insert(movieHandler->movieLists[offset], (void*)movie, movieId);
	moviesNum++;
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
		currMovie->days[day] = InsertScreeningNode(currMovie->days[day], (void*)newScreening, newScreening->hour);


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

// **** Free Methods ****

// Free structure
// O(1) - אין אורך קלט
void FreeMatrix(DaySchedulePtr matrix)
{
	ScreeningPtr* startPtr = *(matrix->screeningsSchedule);
	ScreeningPtr* endPtr = startPtr + NUM_OF_THEATERS * SCREENING_HOURS_PER_DAY - ONE;
	ScreeningPtr* iterPtr = endPtr;
	us currHour;

	// For each element in the matrix
	while (iterPtr >= startPtr)
	{
		currHour = (iterPtr - startPtr) % SCREENING_HOURS_PER_DAY;

		// Free it if the screening starts in the current hour
		if (*iterPtr && (*iterPtr)->hour == currHour)
		{
			//free((*iterPtr)->seats);
			free(*iterPtr);
		}

		// Next
		iterPtr--;
	}
}
// O(n) - אורך הקלט הוא מספר האיברים ברשימה
void FreeList(LLLManager manager)
{
	if (!manager)
		return;
	if (!(manager->next)) {
		free(manager);
		return;
	}

	FreeList(manager->next);
	free(manager);
}
// O(n) - מייצג את מספר ההקרנות במבנה n
void FreeTree(Node* root)
{
	if (!root)
		return;

	FreeTree(root->left);
	FreeTree(root->right);
	
	// Free all screenings in the list in the info (sized m)
	FreeList((LLLManager)root->info);
	
	// Free node
	free(root);
}
// O(n*m) - מייצג את מספר ההקרנות לכל סרט n
// מייצג את מספר הסרטים במבנה m
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
		// Free screening-tree for each day of the movie
		FreeTree(((MoviePtr)movieTree->info)->days[counter]);
	}

	// Free info and node
	free(movieTree->info);
	free(movieTree);
}
// O(n*m) - מייצג את מספר ההקרנות לכל סרט n
// מייצג את מספר הסרטים במבנה m
void FreeAll()
{
	us counter;

	// Free theaters
	for (counter = INIT_VALUE; counter < NUM_OF_THEATERS; counter++)
	{
		//free(theaters[counter]->rowsSeats);
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

// **** More searches ****

// Searches on movies
// O(n) - מייצג את מספר האיברים n עבור עץ
void TreeToListPreOrder(LLLManager* manager, Node* root) {
	if (root) {
		CombineLists(manager, &((LLLNodePtr)root->info));
		TreeToListPreOrder(manager, root->left);
		TreeToListPreOrder(manager, root->right);
	}
}

// O(log(n)) - מייצג את מספר הסרטים n
// מספר ההקרנות המקסימלי הוא קבוע, לכן לא משפיע על הסיבוכיות
LLLManager SearchMovieFromHour(String movieName, us movieId, us day, us hour) {
	// Find current movie by its keys
	MoviePtr currMovie = FindMovie(movieName, movieId);

	// Find the closest hour
	Node* root = ClosestHigherKey(currMovie->days[day], hour);
	LLLManager manager = NULL;

	// Make list from all nodes in the current sub-tree (from the current hour)
	if (root) {
		manager = (LLLManager)root->info;
		TreeToListPreOrder(&manager, root->right);
		return manager;
	}

	return NULL;
	
}

// O(log(n)) - מייצג את מספר הסרטים n
// מספר ההקרנות המקסימלי הוא קבוע, לכן לא משפיע על הסיבוכיות
LLLManager SearchMovieByHour(String movieName, us movieId, us day, us hour) {
	// Find current movie according to its keys
	MoviePtr moviePtr = FindMovie(movieName, movieId);
	Node* screenings = moviePtr->days[day];
	// Find Closest screening hour
	Node* closestMovie = ClosestHigherKey(screenings, hour);

	// Return if not NULL!
	return closestMovie ? (LLLNodePtr)closestMovie->info : NULL;
}

// **** Init Manually Methods ****

void newScreening(int day, MoviePtr movie, us theaterId, int hour) {
	ScreeningPtr screening = (ScreeningPtr)calloc(1 ,sizeof(Screening));
	screening->movie = movie;
	screening->seatsLeft = theaters[theaterId]->totalSeats;
	screening->theaterId = theaterId;
	screening->seats = (Mask*)calloc((theaters[theaterId]->totalSeats - ONE) / sizeof(char) + ONE, sizeof(Mask)); // add malloc to mask
	screening->hour = hour; // ??? redandecy
	int hourOffset;
	int endHour = ceil(hour + movie->length);
	for (hourOffset = hour; hourOffset < endHour; hourOffset++)
	{
		weekSchedule->weekSchedule[day]->screeningsSchedule[theaterId][hourOffset] = screening;
	}
	movie->days[day] = InsertScreeningNode(movie->days[day], (void*)screening, hour);
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
	newDataMovie(1, _strdup("ant man and the wasp"), 1.9);
	newDataMovie(2, _strdup("hulk"), 1.1);
	newDataMovie(3, _strdup("star wars"), 0.7);
	newDataMovie(4, _strdup("kill bill"), 1.1);
	newDataMovie(5, _strdup("twenty one jump street"), 0.6);
	newDataMovie(6, _strdup("spongebob the movie (best)"), 1.3);
	newDataMovie(7, _strdup("avengers 1"), 1.1);
	newDataMovie(8, _strdup("harry poter solving all your problems"), 0.3);
	newDataMovie(9, _strdup("space jam"), 0.7);

	//create theaters
	us* arr1 = (us*)malloc(sizeof(us) * 4); 
	arr1[0] = 6;
	arr1[1] = 10;
	arr1[2] = 15;
	arr1[3] = 20;
	theaters[0] = newDataTheater(0, 4, 20, arr1, 51);
	us* arr2 = (us*)malloc(sizeof(us) * 3);
	arr2[0] = 4;
	arr2[1] = 3;
	arr2[2] = 3;
	theaters[1] = newDataTheater(1, 3, 10, arr2, 16);
	us* arr3 = (us*)malloc(sizeof(us) * 5);
	arr3[0] = 8;
	arr3[1] = 6;
	arr3[2] = 6;
	arr3[3] = 6;
	arr3[4] = 12;
	theaters[2] = newDataTheater(2, 5, 12, arr3, 38);
	us* arr4 = (us*)malloc(sizeof(us) * 2);
	arr4[0] = 5;
	arr4[1] = 5;
	theaters[3] = newDataTheater(3, 2, 5, arr4, 10);
	us* arr5 = (us*)malloc(sizeof(us) * 7);
	arr5[0] = 10;
	arr5[1] = 10;
	arr5[2] = 10;
	arr5[3] = 10;
	arr5[4] = 10;
	arr5[5] = 10;
	arr5[6] = 10;
	theaters[4] = newDataTheater(4, 7, 10, arr5, 60);

	// create screening
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

// **** Tickets Handlers ****

// Movie Seats
void ShowMovieSeats(ScreeningPtr screenPtr)
{
	int seatsInRow;
	int bit;
	us totalSeats = ZERO;
	int row;
	MovieTheaterPtr th = theaters[screenPtr->theaterId];

	// For each row
	for (us rowOffset = ZERO; rowOffset < th->rowNum; rowOffset++)
	{
		seatsInRow = th->rowsSeats[rowOffset];

		// For each seat
		for (us seat = ZERO; seat < seatsInRow; seat++)
		{
			// PrintBit
			bit = (screenPtr->seats[(totalSeats / EIGHT)] & (ONE << totalSeats % EIGHT)) >> (totalSeats % EIGHT);
			printf("%d", bit);

			totalSeats++;
		}
		printf("\n");
	}
}

void PickMovieSeats(ScreeningPtr screenPtr, us seat)
{
	screenPtr->seats[(seat / EIGHT)] |= CHAIR_TAKEN << (seat % EIGHT);
	screenPtr->seatsLeft--;
}

// show the theater avilable seats so you will know where you can and cant buy seats.
void ShowAvilableSeats(char* movieName, us movieId, us hour, us day) {
	MoviePtr movie = FindMovie(movieName, movieId);
	LLLNodePtr listNode = (((LLLManager)Search(movie->days[day], hour)->info));
	while (((ScreeningPtr)listNode->info)->seatsLeft == 0)
	{
		listNode = listNode->next;
	}
	ShowMovieSeats(((ScreeningPtr)listNode->info));
}

// Show movie Ticket By given screening and seet only Ui funcsion
void ShowMovieTicket(ScreeningPtr screenPtr, us chosenSeat)
{
	printf("\n");
	printf("=========================================================\n");
	String movie = screenPtr->movie->name;
	printf("movie name: %s\n", movie);

	us startTime = screenPtr->hour;
	printf("movie start in: %hu pm \n", startTime);
	us rowNumber = 0;
	us sum = (theaters[screenPtr->theaterId])->rowsSeats[rowNumber];
	if (sum < chosenSeat) {
		for (rowNumber = ONE; sum < chosenSeat; sum += (theaters[screenPtr->theaterId])->rowsSeats[rowNumber], rowNumber++);
	}
	printf("your seat in: \n");
	printf("row: %hu seat: %hu\n", rowNumber, chosenSeat);
	printf("=========================================================\n");
}

//O(mlogmlogn) n - number of movies m - number of screening in given hour 
// Buy Movie Ticket by movie name, id, hour and day + seat you want.
void BuyTicket(char* movieName, us movieId, us seat, us hour, us day) {
	MoviePtr movie = FindMovie(movieName, movieId); // find movie ptr by movie name and id
	LLLNodePtr listNode = (((LLLManager)Search(movie->days[day], hour)->info));
	while (((ScreeningPtr)listNode->info)->seatsLeft == 0) // cheack if there is avilable seats in this screening
	{
		listNode = listNode->next;
	}
	ScreeningPtr screening = ((ScreeningPtr)listNode->info);
	PickMovieSeats(screening, seat);
	ShowMovieTicket(screening, seat);
}



// Print Movie Tree
// O(N) מספר הסרטים N
void printMovieCodeTree(Node* movie) {
	if (movie)
	{
		printf("movie name: %s | code: %d\n", ((MoviePtr)(movie->info))->name, ((MoviePtr)(movie->info))->movieId);
		printMovieCodeTree(movie->left);
		printMovieCodeTree(movie->right);
	}
}

// Show all movies O(N)
// מספר הסרטים N
void printMovieCodes() {
	us firstChar;

	// For all open hashes
	for ( firstChar = INIT_VALUE; firstChar < NUM_OF_CHARS; firstChar++)
	{
		// Print all movies
		printMovieCodeTree(movieHandler->movieLists[firstChar]);
	}
}

int main()
{
	InitData();

	// Show movies
	printf(" ------ ALL Movies -----\n");
	printMovieCodes();
	printf("\n\n");

	// Search all first day
	printf(" ------ Movies In firstDay -----\n");
	PrintScreeningsList(SearchDay(weekSchedule->weekSchedule, 0));
	printf("\n\n");

	// Search first day from hour 3
	printf(" ------ Movies In firstDay from hour 3 -----\n");
	PrintScreeningsList(SearchDayFromHour(weekSchedule->weekSchedule, 0, 3));
	printf("\n\n");

	// Search first day in hour 4
	printf(" ------ Movies In firstDay in hour 4 -----\n");
	PrintScreeningsList(SearchDayHour(weekSchedule->weekSchedule, 0, 4));
	printf("\n\n");

	// Movie Seats in theater 2 hour 5
	printf(" ------ Seats in first day in hour 5 -----\n");
	ShowAvilableSeats("star wars", 3, 5, 0);
	printf("\n\n");

	// Buy new ticket
	printf(" ------ But seat in first day in hour 5 of star wars -----\n");
	BuyTicket("start wars", 3, 2, 5, 0);
	printf("\n\n");

	// Movie Seats in theater 2 hour 5
	printf(" ------ Seats in first day in hour 5 -----\n");
	ShowAvilableSeats("star wars", 3, 5, 0);
	printf("\n\n");

	weekSchedule->weekSchedule[0]->screeningsSchedule[0][4]->seatsLeft -= 6;
	weekSchedule->weekSchedule[0]->screeningsSchedule[1][1]->seatsLeft -= 3;
	weekSchedule->weekSchedule[3]->screeningsSchedule[0][4]->seatsLeft -= 6;
	weekSchedule->weekSchedule[3]->screeningsSchedule[1][1]->seatsLeft -= 3;
	weekSchedule->weekSchedule[0]->screeningsSchedule[0][4]->seatsLeft -= 6;
	weekSchedule->weekSchedule[0]->screeningsSchedule[1][1]->seatsLeft -= 3;
	weekSchedule->weekSchedule[3]->screeningsSchedule[0][4]->seatsLeft -= 6;
	weekSchedule->weekSchedule[3]->screeningsSchedule[1][1]->seatsLeft -= 3;
	WeekSummery weekSummery = MakeWeekSummery(theaters, weekSchedule, moviesNum);

	// Bussiest day
	printf(" ------ Bussiest day -----\n");
	printf("%d", FindBussiestDay(&weekSummery, moviesNum));
	printf("\n\n");

	// Bussiest day
	printf(" ------ Bussiest day -----\n");
	printf("%d", FindBussiestDay(&weekSummery, moviesNum));
	printf("\n\n");

	// Most wanted movie
	printf(" ------ Most wanted movie -----\n");
	printf("%d", FindMostWantedMovie(&weekSummery, moviesNum));
	printf("\n\n");

	// The confidence interval
	printf(" ------ Expectations for a future day -----\n");
	double lower = 0;
	double upper = 0;
	ExpectationsPerDay(&weekSummery, moviesNum, &lower, &upper);
	printf("The confidence interval is between %lf - %lf\n\n", lower, upper);


	FreeAll();
}

/*void PrintList(LLLManager manager)
{
	if (!manager)
		return;
	if (!manager->next) {
		PrintScreening((ScreeningPtr)manager->info);
		return;
	}

	PrintList(manager->next);
	PrintScreening((ScreeningPtr)manager->info);
}
void PrintTree(Node* root)
{
	if (!root)
		return;

	PrintTree(root->left);
	PrintTree(root->right);
	PrintList((LLLManager)root->info);
}
void PrintMovieTree(Node* movieTree)
{
	us counter;

	if (!movieTree)
		return;

	// PostOrder
	PrintMovieTree(movieTree->left);
	PrintMovieTree(movieTree->right);

	// Free day-trees
	printf("Movie %s:\n", ((MoviePtr)movieTree->info)->name);
	for (counter = INIT_VALUE; counter < NUM_OF_DAYS_IN_WEEK; counter++)
	{
		printf("Day number %d:\n", counter);
		PrintTree(((MoviePtr)movieTree->info)->days[counter]);
	}
}*/