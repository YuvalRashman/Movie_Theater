#include "AVLHandler.h"

ScreeningPtr SearchScreeningByMovieId(int movieId);


// what if the left is bigger
Node* SearchClose(Node* root, int key) {
	if (!root)
	{
		return root;
	}
	if (root->right)
	{
		if (root->right->key < key) // if the right key is smaller then yours go there
		{
			return SearchClose(root->right, smallest ,key);
		}
	}
	if (root->left)
	{
		if (root->left->key > key) {
			return SearchClose(root->left, key);
		}
	}
	// need to check by the root from now to check if good
	if (key > root->key) {
		SearchClose(root->right, key);
	}
	return root;
}


Node* closestKey(struct Node* node, int key) {
    if (node)
        return NULL;  // Return maximum integer value if the node is empty

    int diff = node->key - key;

    if (diff == 0)
        return node;

    if (diff > 0) {
        Node* closestLeft = closestKey(node->left, key);

        if (abs(node->key - key) < abs(closestLeft->key - key))
            return node;
        else
            return closestLeft;
    } else {
        Node* closestRight = closestKey(node->right, key);

        if (abs(node->key - key) < abs(closestRight->key - key))
            return node;
        else
            return closestRight;
    }
}


ScreeningPtr SearchScreeningByMovieId(int movieId, int day, int hour) {
	MoviePtr moviePtr = movies[movieId];
	Node* screenings = moviePtr->days[day];
	Search();
}

