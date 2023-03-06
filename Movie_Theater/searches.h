#include "AVLHandler.h"

// **** Useful methods ****

// Hash for movies - open hash
// O(1) - אין אורך קלט
us hash(String movie)
{
    return *(movie)-LOW_A;
}

// O(n) - אורך הקלט הוא מספר האיברים בוקטור
us SumVec(us* vecStartPtr, us* vecEndPtr, us delta)
{
    us* iterPtr = vecStartPtr;
    us sum = INIT_VALUE;

    // For all elements
    while (iterPtr <= vecEndPtr)
    {
        // Sum
        sum += *iterPtr;

        // Next
        iterPtr += delta;
    }

    return sum;
}

// **** Searches ****

// O(log(n)) - אורך הקלט המייצג את מספר הצמתים בעץ n
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
// O(log(n)) - אורך הקלט המייצג את מספר הצמתים בעץ n
// NULL הפעולה עוטפת את הפעולה הרקורסיבית ומביאה לה
Node* ClosestSmallerKey(Node* node, int key) {
    return ClosestSmallerKeyRec(node, key, NULL);
}

// O(log(n)) - אורך הקלט המייצג את מספר הצמתים בעץ n
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
// O(log(n)) - אורך הקלט המייצג את מספר הצמתים בעץ n
// NULL הפעולה עוטפת את הפעולה הרקורסיבית ומביאה לה
Node* ClosestHigherKey(Node* node, int key) {
    return ClosestHigherKeyRec(node, key, NULL);
}

// Find movies by a requested day and hour
// O(1) - אין אורך קלט, המעבר על מטריצה בגודל קבוע
LLLManager SearchDayHour(WeekSchedulePtr weekSchedule, us day, us hour)
{
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	ScreeningPtr* iterPtr = daySchedule->screeningsSchedule[INIT_VALUE] + hour;
	ScreeningPtr* endPtr = daySchedule->screeningsSchedule[NUM_OF_THEATERS - ONE] + hour;
	LLLManager managerLLL;

    // Init returned list
	InitLLL(&managerLLL);

    // Find all screenings in that hour and day
	while (iterPtr <= endPtr)
	{
        // If the screening exists and starts in the specific hour
		if (*iterPtr && (*iterPtr)->hour == hour)
			PushLLL(&managerLLL, (void*)*iterPtr);

        // delta is whole row (the function loops on the column of the matrix)
		iterPtr += SCREENING_HOURS_PER_DAY;
	}

	return managerLLL;
}

// Find movies by day from a request hour
// O(1) - אין אורך קלט, המעבר על מטריצה בגודל קבוע
LLLManager SearchDayFromHour(WeekSchedulePtr weekSchedule, us day, us hour)
{
	DaySchedulePtr daySchedule = weekSchedule->weekSchedule[day];
	LLLManager managerLLL;
	LLLManager temp;
	int hourIter = hour;

    // Init returned list
	InitLLL(&managerLLL);

    // Loop from the current hour to the end
	while (hourIter < SCREENING_HOURS_PER_DAY)
	{
        // Find all movies in the current hour and day
		temp = SearchDayHour(weekSchedule, day, hourIter);

        // Add the returned list into our list
		CombineLists(&managerLLL, &temp);

		hourIter++;
	}

	return managerLLL;
}

// Search all movies in a day
// O(1) - אין אורך קלט, המעבר על מטריצה בגודל קבוע
LLLManager SearchDay(WeekSchedulePtr weekSchedule, us day)
{
    // Search all screenings in the day from hour 0
	SearchDayFromHour(weekSchedule, day, INIT_VALUE);
}

// Print screening

// Print nicly a screening to the user
// O(1) - אין אורך קלט
void PrintScreening(ScreeningPtr screeningPtr)
{
    printf("ID(%d): %s - Starting hour = %d in theater %d!\n", screeningPtr->movie->movieId, screeningPtr->movie->name, screeningPtr->hour, screeningPtr->theaterId);
}

// Print all screenings in a list to the user
// O(n) - אורך הקלט כאשר הוא מייצג את מספר הצמתים ברשימה n
void PrintScreeningsList(LLLManager screeningsList)
{
    LLLNodePtr iterPtr = screeningsList;
    ScreeningPtr temp;

    // Loop on all nodes
    while (iterPtr)
    {
        // Find current screening
        temp = (ScreeningPtr)iterPtr->info;
        
        // If there are any seats left the screening is valid for the user
        if (temp->seatsLeft)
            PrintScreening(temp);

        // Get next
        iterPtr = iterPtr->next;
    }
}