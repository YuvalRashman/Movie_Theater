#include "AVLHandler.h"
#include "Structs.h"

ScreeningPtr SearchScreeningByMovieId(int movieId);


Node* ClosestSmallerKeyRec(Node* node, int key, Node *best) {
    if (!node) { // if NULL
        return best; // return the best so far
    }
    if (node->key == key) { // if equal we neeed to find smaller or best / father
        if (node->left) {
            return ClosestSmallerKey(node->left, key, node->left);
        }
        return best;
    }
    if (node->key < key) { // if less need to change the best
        return ClosestSmallerKey(node->right, key, node);
    }
    return ClosestSmallerKey(node->left, key, best); // need to check the left sub tree
}

Node* ClosestSmallerKey(Node* node, int key) {
    return ClosestSmallerKeyRec(node, key, NULL);
}


ScreeningPtr SearchScreeningByMovieId(int movieId, int day, int hour) {
	MoviePtr moviePtr = movies[movieId];
	Node* screenings = moviePtr->days[day];
	return ClosestSmallerKey(screenings, hour)->info;
}

