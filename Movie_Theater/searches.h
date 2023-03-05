#include "AVLHandler.h"

// O(log(n))
Node* ClosestSmallerKeyRec(Node* node, int key, Node* best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if (node->key == key) { // if equal we neeed to find smaller or best / father
        return node;
    }
    if (node->key < key) { // if less need to change the best
        return ClosestSmallerKeyRec(node->right, key, node);
    }
    return ClosestSmallerKeyRec(node->left, key, best); // need to check the left sub tree
}
Node* ClosestSmallerKey(Node* node, int key) {
    return ClosestSmallerKeyRec(node, key, NULL);
}

// O(log(n))
Node* ClosestHigherKeyRec(Node* node, int key, Node* best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if (node->key == key) { // if equal we neeed to find smaller or best / father
        return node;
    }
    if (node->key > key) { // if less need to change the best
        return ClosestHigherKeyRec(node->left, key, node);
    }
    return ClosestHigherKeyRec(node->right, key, best); // need to check the left sub tree
}
Node* ClosestHigherKey(Node* node, int key) {
    return ClosestHigherKeyRec(node, key, NULL);
}

// O(n) - מייצג את מספר האיברים n עבור עץ
void TreeToListInOrder(LLLManager* manager, Node* root) {
    if (!root) {
        TreeToListInOrder(manager, root->left);
        CombineLists(manager, &((LLLNodePtr)root->info));
        TreeToListInOrder(manager, root->right);
    }
}

// Find movies by request day and hour
// O(1)
LLLManager SearchDayHour(WeekSchedulePtr weekSchedule, us day, us hour)
{
	// create an array of screenings
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	ScreeningPtr* iterPtr = daySchedule->screeningsSchedule[INIT_VALUE] + hour;
	ScreeningPtr* endPtr = daySchedule->screeningsSchedule[NUM_OF_THEATERS - ONE] + hour;
	LLLManager managerLLL;

	InitLLL(&managerLLL);

    // Find all screenings in that hour
	while (iterPtr <= endPtr)
	{
        // If the screening starts in the specific hour
		if (*iterPtr && (*iterPtr)->hour == hour)
			PushLLL(&managerLLL, (void*)*iterPtr);

		iterPtr += SCREENING_HOURS_PER_DAY;
	}

	return managerLLL;
}

//O(1)
LLLManager SearchDayFromHour(WeekSchedulePtr weekSchedule, us day, us hour)
{
	// create an array of screenings
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	LLLManager managerLLL;
	LLLManager temp;
	int hourIter = hour;

	InitLLL(&managerLLL);

    // Loop from the current hour to the end
	while (hourIter <= SCREENING_HOURS_PER_DAY)
	{
		temp = SearchDayHour(weekSchedule, day, hourIter);
		CombineLists(&managerLLL, &temp);

		hourIter++;
	}

	return managerLLL;
}

//O(1)
LLLManager SearchDay(WeekSchedulePtr weekSchedule, us day)
{
    // Search all screenings in the day from hour 0
	SearchDayFromHour(weekSchedule, day, INIT_VALUE);
}

// Print screening

// O(1)
void PrintScreening(ScreeningPtr screeningPtr)
{
    printf("%s: starting hour = %d in theater %d\n", screeningPtr->movie->name, screeningPtr->hour, screeningPtr->theaterId);
}

// O(1)
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