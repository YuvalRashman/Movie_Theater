#include "CONSTANTS.h"
#include "AVLHandler.h"
#include "Structs.h"
#include "LLLHandler.h"


ScreeningPtr SearchScreeningByMovieId(int movieId);

Node* ClosestSmallerKeyRec(Node* node, int key, Node* best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if (node->key == key) { // if equal we neeed to find smaller or best / father
        /*if (node->left) {
            return ClosestSmallerKey(node->left, key, node->left);
        }*/
        return best;
    }
    if (node->key < key) { // if less need to change the best
        return ClosestSmallerKeyRec(node->right, key, node);
    }
    return ClosestSmallerKeyRec(node->left, key, best); // need to check the left sub tree
}

Node* ClosestSmallerKey(Node* node, int key) {
    return ClosestSmallerKeyRec(node, key, NULL);
}

Node* ClosestHigherKeyRec(Node* node, int key, Node* best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if (node->key == key) { // if equal we neeed to find smaller or best / father
        /*if (node->right) {
            return ClosestSmallerKey(node->right, key, node->right);
        }*/
        return best;
    }
    if (node->key > key) { // if less need to change the best
        return ClosestHigherKeyRec(node->left, key, node);
    }
    return ClosestHigherKeyRec(node->right, key, best); // need to check the left sub tree
}

Node* ClosestHigherKey(Node* node, int key) {
    return ClosestHigherKeyRec(node, key, NULL);
}

void AddScreeningsToListInOrder(LLLManager* manager, Node* root) {
    if (!root) {
        AddScreeningsToListInOrder(manager, root->left);
        PushLLL(manager, root->info);
        AddScreeningsToListInOrder(manager, root->right);
    }
}

LLLManager* getAllScreeningsAfter(int movieId, int day, int hour) {
    Node* root = ClosestHigherKey(movies[movieId]->days[day], hour);
    LLLManager* manager = (LLLManager*)malloc(sizeof(LLLManager));
    while ((ScreeningPtr)(root->info)->hour == hour)
    {
        PushLLL(manager, root->info);
        root = root->right;
    }
    AddScreeningsToListInOrder(manager, root);
    return manager;
}

ScreeningPtr SearchCloseScreeningByMovieId(int movieId, int day, int hour) {
    MoviePtr moviePtr = movies[movieId];
    Node* screenings = moviePtr->days[day];
    return ClosestSmallerKey(screenings, hour)->info;
}

// Find movies by request day and hour
// O(1)
LLLManager SearchDayHour(WeekSchedulePtr weekSchedule, us day, int hour)
{
	// create an array of screenings
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	ScreeningPtr* iterPtr = daySchedule->screeningsSchedule[INIT_VALUE][hour];
	ScreeningPtr* endPtr = daySchedule->screeningsSchedule[NUM_OF_THEATERS - ONE][hour];
	LLLManager managerLLL;

	InitLLL(managerLLL);

	while (iterPtr <= endPtr)
	{
		if ((*iterPtr)->hour == hour)
			PushLLL(&managerLLL, (void*)*iterPtr);

		iterPtr += SCREENING_HOURS_PER_DAY;
	}

	return managerLLL;
}

//O(1)
LLLManager SearchClosestDayHour(WeekSchedulePtr weekSchedule, us day, int hour)
{
	// create an array of screenings
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	LLLManager managerLLL;
	LLLManager temp;
	int hourIter = hour;
	InitLLL(managerLLL);


	while (hourIter <= SCREENING_HOURS_PER_DAY)
	{
		temp = SearchDayHour(weekSchedule, day, hourIter);
		CombineLists(&managerLLL, &temp);

		hourIter++;
	}

	return managerLLL;
}

//O(1)
LLLManager SearchDay(WeekSchedulePtr weekSchedule, us day, int hour)
{
	SearchClosestDayHour(weekSchedule, day, INIT_VALUE);
}

// Print screening

void PrintScreening(ScreeningPtr screeningPtr)
{
    printf("%s: starting hour = %d in theater %d", screeningPtr->movie, screeningPtr->hour, screeningPtr->theaterId);
}

void PrintScreeningsList(LLLManager screeningsList)
{
    LLLNodePtr iterPtr = screeningsList;
    ScreeningPtr temp;

    while (iterPtr)
    {
        temp = (ScreeningPtr)iterPtr->info;
        
        if (temp->seatsLeft)
            PrintScreening(temp);

        iterPtr = iterPtr->next;
    }
}