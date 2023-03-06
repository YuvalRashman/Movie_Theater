#include <stdlib.h>
#include <stdio.h>

typedef short Bool;

typedef struct LLL {
	void* info;
	struct LLL* next;
} LLL, *LLLNodePtr, *LLLManager, **LLLManagerPtr;

// O(1)
void InitLLL(LLLManager* managerPtr)
{
	*managerPtr = NULL;
}

// O(1)
void PushLLL(LLLManager* managerPtr, void* info)
{
	LLLNodePtr nodePtr = (LLLNodePtr)malloc(sizeof(LLL));
	nodePtr->info = info;
	nodePtr->next = *managerPtr;
	*managerPtr = nodePtr;
}

// O(1)
void AddAfterLLL(LLLNodePtr nodePtr, void* info)
{
	LLLNodePtr newNodePtr = (LLLNodePtr)malloc(sizeof(LLL));
	newNodePtr->info = info;
	newNodePtr->next = nodePtr->next;
	nodePtr->next = newNodePtr;
}

// O(1)
void PopLLL(LLLManager* managerPtr)
{
	LLLNodePtr temp = *managerPtr;
	*managerPtr = temp->next;
	free(temp->info);
	free(temp);
}

// O(1)
void DeleteAfterLLL(LLLNodePtr nodePtr)
{
	LLLNodePtr temp = nodePtr->next;
	nodePtr->next = temp->next;
	free(temp->info);
	free(temp);
}

// O(1)
Bool IsEmptyLLL(LLLManager managerPtr)
{
	return !managerPtr;
}

// O(min(n,m))
// m - מספר הצמתים ברשימה ראשונה
// n - מספר הצמתים ברשימה שנייה
void CombineLists(LLLManagerPtr managerPtr, LLLManagerPtr secondManagerPtr)
{
	LLLNodePtr lllIter = *managerPtr;

	// If first list is empty
	if (IsEmptyLLL(lllIter))
		*managerPtr = *secondManagerPtr;
	else {
		// Get end of first list
		while (lllIter->next)
		{
			lllIter = lllIter->next;
		}

		// Combine lists
		lllIter->next = *secondManagerPtr;
	}
}