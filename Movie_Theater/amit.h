#pragma once

// O(n)
// T(n) = 2T(n / 2) + O(1)
// Print AVL Tree:
void PrintMoviesScejTree(Node* iNode)
{
    if (!iNode)
    {
        return;
    }
    else
    {
        printf("%s", iNode->info);
        PrintMoviesScejTree(iNode->left);
        PrintMoviesScejTree(iNode->right);
    }
}

int getMovieScreeningTime(MoviePtr movie, int day, int hour)
{
	// check if movie is NULL
	if (movie == NULL) {
		return ZERO;
	}
	// get the AVL tree of the requested day from the movie
	Node* node = movie->days[day];
	// if the tree is empty, the movie is not available
	if (node == NULL) {
		return ZERO;
	}
	// check the left most node in the tree
	while (node->left != NULL) {
		node = node->left;
	}
	// check the right most node in the tree
	while (node->right != NULL) {
		node = node->right;
	}
	// check if the requested time is within the range of the screening time
	if (hour > node->key) {
		return ZERO;
	}
	else {
		return node->key;
	}
}


// returns an array of movie screenings closest to the requested hour on the requested day
// O (n)
ScreeningPtr* getClosestScreenings(int day, int hour)
{
	// create an array of screenings
	ScreeningPtr* screenings = (ScreeningPtr)malloc(sizeof(ScreeningPtr)MAXSCREENINGS));
	int count = ZERO; // counter to keep track of the screenings
	// traverse the weekSchedule
	for (int i = ZERO; i < NUM_OF_DAYS_IN_WEEK; i++) {
		// traverse the daySchedule
		for (int j = ZERO; j < SCREENING_HOURS_PER_DAY; j++) {
			// traverse the screeningsSchedule
			for (int k = ZERO; k < NUM_OF_THEATERS; k++) {
				// get the screening
				ScreeningPtr screening = WeekSchedulePtr->weekSchedule[i]->screeningsSchedule[j][k];
				// check if the screening is on the requested day
				if (day == screening->movie->days[i]->key) {
					// check if the screening is closest to the requested hour
					if (abs(hour - screening->hour) <= ONE) {
						screenings[count] = screening;
						count++;
					}
				}
			}
		}
	}
	return screenings;
}


// Returns an array of movie screenings closest to a certain time on a requested day and also to a certain movie
// Time Complexity: O(n)
ScreeningPtr* getClosestScreeningsByMovie(MoviePtr movie, int day, int hour) {
	// create an array of screenings
	ScreeningPtr* screenings = (ScreeningPtr)malloc(sizeof(ScreeningPtr)MAXSCREENINGS));
	int count = ZERO; // counter to keep track of the screenings
	// traverse the weekSchedule
	for (int i = ZERO; i < NUMOFDAYSINWEEK; i++) {
		for (int j = ZERO; j < SCREENINGHOURSPERDAY; j++) {
			for (int k = ZERO; k < NUMOFTHEATERS; k++) {
				ScreeningPtr screening = WeekSchedulePtr->weekSchedule[i]->screeningsSchedule[j][k];
				// check if the screening is on the requested day
				if (day == screening->movie->days[i]->key && movie->MovieId == screening->movie->MovieId) {
					// check if the screening is closest to the requested hour
					if (abs(hour - screening->hour) <= ONE)
					{
						screenings[count] = screening;
						count++;
					}
				}
			}
		}
	}
	return screenings;
}


// Find movies by request day.
// O(n)
ScreeningPtr* getClosestScreeningsByDay(int day, int hour)
{
	// create an array of screenings
	ScreeningPtr* screenings = (ScreeningPtr)malloc(sizeof(ScreeningPtr)MAXSCREENINGS));
	int count = ZERO; // counter to keep track of the screenings
	// traverse the weekSchedule
	for (int i = ZERO; i < NUMOFDAYSINWEEK; i++) {
		for (int j = ZERO; j < SCREENINGHOURSPERDAY; j++) {
			for (int k = ZERO; k < NUMOFTHEATERS; k++) {
				ScreeningPtr screening = WeekSchedulePtr->weekSchedule[i]->screeningsSchedule[j][k];
				// check if the screening is on the requested day
				if (day == screening->movie->days[i]->key) {
					// check if the screening is closest to the requested hour
					if (abs(hour - screening->hour) <= ONE) {
						screenings[count] = screening;
						count++;
					}
				}
			}
		}
	}

	return screenings;
}


// Returns an array of movie screenings closest to a certain time on a requested hour
// O(n)
ScreeningPtr* getClosestScreeningsByHour(int hour) {
	// create an array of screenings
	ScreeningPtr* screenings = (ScreeningPtr)malloc(sizeof(ScreeningPtr)MAXSCREENINGS));
	int count = ZERO; // counter to keep track of the screenings
	for (int i = ZERO; i < NUMOFDAYSINWEEK; i++) {
		for (int j = ZERO; j < SCREENINGHOURSPERDAY; j++) {
			for (int k = ZERO; k < NUMOFTHEATERS; k++) {
				ScreeningPtr screening = WeekSchedulePtr->weekSchedule[i]->screeningsSchedule[j][k];
				// check if the screening is closest to the requested hour
				if (abs(hour - screening->hour) <= ONE) {
					screenings[count] = screening;
					count++;
				}
			}
		}
	}
	return screenings;
}

