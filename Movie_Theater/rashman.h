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

ScreeningPtr SearchScreeningByMovieId(int movieId, int day, int hour) {
	MoviePtr moviePtr = movies[movieId];
	Node* screenings = moviePtr->days[day];
	Search();
}

