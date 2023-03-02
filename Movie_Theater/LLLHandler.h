#include <stdlib.h>
#include <stdio.h>
#include <C:\Users\nadavru\Documents\GitHub\Movie_Theater\Movie_Theater\Structs.h>

typedef struct LLL {
	void* info;
	LLL* next;
} *LLLManager, * LLLNodePtr;

void InitLLL(LLLManager* managerPtr)
{
	*managerPtr = NULL;
}
void PushLLL(LLLManager* managerPtr, void* info)
{
	LLLNodePtr nodePtr = (LLLNodePtr)malloc(sizeof(LLL));
	nodePtr->info = info;
	nodePtr->next = *managerPtr;
	*managerPtr = nodePtr;
}
void AddAfterLLL(LLLNodePtr nodePtr, void* info)
{
	LLLNodePtr newNodePtr = (LLLNodePtr)malloc(sizeof(LLL));
	newNodePtr->info = info;
	newNodePtr->next = nodePtr->next;
	nodePtr->next = newNodePtr;
}
void PopLLL(LLLManager* managerPtr)
{
	LLLNodePtr temp = *managerPtr;
	*managerPtr = temp->next;
	free(temp->info);
	free(temp);
}
void DeleteAfterLLL(LLLNodePtr nodePtr)
{
	LLLNodePtr temp = nodePtr->next;
	nodePtr->next = temp->next;
	free(temp->info);
	free(temp);
}
Bool IsEmpty(LLLManager managerPtr)
{
	return managerPtr == NULL;
}