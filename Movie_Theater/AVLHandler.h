// Avl handler
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Structs.h"

typedef short Bool;
typedef unsigned short us;

// O(1)
Bool IsLeaf(Node* root)
{
    return !root->left & !root->right;
}

// Create a new node with the given key
// O(1)
Node* NewNode(void* info, us key) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->info = info;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->key = key;
    return newNode;
}

// Get the height of the node
// O(1)
us GetHeight(Node* root) {
    if (!root) {
        return ZERO;
    }

    return root->height;
}

// Get the balance factor of the node
// O(1)
short GetBF(Node* root) {
    if (!root) {
        return ZERO;
    }

    return GetHeight(root->left) - GetHeight(root->right);
}

// Rotate the node to the left
// O(1)
Node* RR(Node* root) {
    Node* right = root->right;

    // Exchange
    root->right = right->left;
    right->left = root;
    
    // Update heights
    root->height = 1 + fmax(GetHeight(root->left), GetHeight(root->right));
    right->height = 1 + fmax(GetHeight(right->left), GetHeight(right->right));

    return right;
}

// Rotate the node to the right
// O(1)
Node* LL(Node* root) {
    Node* left = root->left;

    // Echange
    root->left = left->right;
    left->right = root;

    // Update heights
    root->height = 1 + fmax(GetHeight(root->left), GetHeight(root->right));
    left->height = 1 + fmax(GetHeight(left->left), GetHeight(left->right));

    return left;
}

// Insert a key into the tree
// O(log(n))
Node* Insert(Node* root, void* info, us newKey) {
    // Root is empty
    if (!root) {
        return NewNode(info, newKey);
    }

    short currBF;
    us currKey = root->key;

    // Go left
    if (newKey < currKey) {
        root->left = Insert(root->left, info, newKey);
    }
    // Go right
    else {
        root->right = Insert(root->right, info, newKey);
    }

    // update height and the balance factor of the current node
    root->height = 1 + fmax(GetHeight(root->left), GetHeight(root->right));
    currBF = GetBF(root);

    // Perform rotations
    // LL
    if (currBF > 1 && newKey < root->left->key) {
        return LL(root);
    }
    // RR
    else if (currBF < -1 && newKey > root->right->key) {
        return RR(root);
    }
    // LR
    else if (currBF > 1 && newKey > root->left->key) {
        root->left = RR(root->left);
        return LL(root);
    }
    // RL
    else if (currBF < -1 && newKey < root->right->key) {
        root->right = LL(root->right);
        return RR(root);
    }

    return root;
}

// Insert a key to the avl - but for the screening tree
Node* NewScreeningNode(void* info, us key) {
    LLLManager manager;
    Node* newNode = (Node*)malloc(sizeof(Node));

    InitLLL(&manager);
    PushLLL(&manager, info);
    newNode->info = (void*) manager;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->key = key;
    return newNode;
}
Node* InsertScreeningNode(Node* root, void* info, us newKey) {
    // Root is empty
    if (!root) {
        return NewScreeningNode(info, newKey);
    }
    if (root->key == newKey)
    {
        PushLLL(&(LLLManager)root->info, info);
        return root;
    }

    short currBF;
    us currKey = root->key;

    // Go left
    if (newKey < currKey) {
        root->left = InsertScreeningNode(root->left, info, newKey);
    }
    // Go right
    else {
        root->right = InsertScreeningNode(root->right, info, newKey);
    }

    // update height and the balance factor of the current node
    root->height = 1 + fmax(GetHeight(root->left), GetHeight(root->right));
    currBF = GetBF(root);

    // Perform rotations
    // LL
    if (currBF > 1 && newKey < root->left->key) {
        return LL(root);
    }
    // RR
    else if (currBF < -1 && newKey > root->right->key) {
        return RR(root);
    }
    // LR
    else if (currBF > 1 && newKey > root->left->key) {
        root->left = RR(root->left);
        return LL(root);
    }
    // RL
    else if (currBF < -1 && newKey < root->right->key) {
        root->right = LL(root->right);
        return RR(root);
    }

    return root;
}

// Search for a key in the tree
// O(log(n))
Node* Search(Node* root, us key) {
    if (!root) {
        return root;
    }
    else {
        us currKey = root->key;

        if (key < currKey) {
            return Search(root->left, key);
        }
        else if (key > currKey) {
            return Search(root->right, key);
        }
        else {
            return root;
        }
    }
}

// O(log(n))
Node* FindMin(Node* root)
{
    if (IsLeaf(root))
        return root;
    else {
        return FindMin(root->left);
    }
}

// O(log(n))
Node* FindMax(Node* root)
{
    if (IsLeaf(root))
        return root;
    else {
        return FindMin(root->right);
    }
}
