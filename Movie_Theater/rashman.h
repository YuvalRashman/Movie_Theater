#include "AVLHandler.h"
#include "Structs.h"
#include "LLLHandler.h"

ScreeningPtr SearchScreeningByMovieId(int movieId);

Node* ClosestSmallerKeyRec(Node* node, int key, Node *best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if ((ScreeningPtr))(node->info)->hour == key) { // if equal we neeed to find smaller or best / father
        /*if (node->left) {
            return ClosestSmallerKey(node->left, key, node->left);
        }*/
        return best;
    }
    if ((ScreeningPtr)(node->info)->hour < key) { // if less need to change the best
        return ClosestSmallerKey(node->right, key, node);
    }
    return ClosestSmallerKey(node->left, key, best); // need to check the left sub tree
}

Node* ClosestHigherKeyRec(Node* node, int key, Node *best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if ((ScreeningPtr)(node->info)->hour == key) { // if equal we neeed to find smaller or best / father
        /*if (node->right) {
            return ClosestSmallerKey(node->right, key, node->right);
        }*/
        return best;
    }
    if ((ScreeningPtr)(node->info)->hour > key) { // if less need to change the best
        return ClosestSmallerKey(node->left, key, node);
    }
    return ClosestSmallerKey(node->right, key, best); // need to check the left sub tree
}

Node* ClosestSmallerKey(Node* node, int key) {
    return ClosestSmallerKeyRec(node, key, NULL);
}

Node* ClosestHigherKey(Node* node, int key) {
    return ClosestHigherKeyRec(node, key, NULL);
}


void AddScreeningsToListInOrder(LLLManager* manager, Node* root){
    if (!root) {
        AddScreeningsToListInOrder(manager ,root->left);
        PushLLL(manager, root);
        AddScreeningsToListInOrder(manager ,root->right);
    }
}

LLLManager* getAllScreeningsAfter(int movieId ,int day, int hour) {
    Node* root = ClosestHigherKey(movies[movieId]->days[day], hour);
    LLLManager* manager = (LLLManager*) malloc(sizeof(LLLManager));
    AddScreeningsToListInOrder(manager, root);
    return manager;
}


ScreeningPtr SearchScreeningByMovieId(int movieId, int day, int hour) {
	MoviePtr moviePtr = movies[movieId];
	Node* screenings = moviePtr->days[day];
	return ClosestSmallerKey(screenings, hour)->info;
}

