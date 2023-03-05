// Avl handler
#include <stdio.h>
#include <stdlib.h>
#include <Math.h>
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
us GetBF(Node* root) {
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

    us currBF;
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

// O(1)
void* DupInfo(void* info)
{
    ScreeningPtr curr = (ScreeningPtr)info;
    ScreeningPtr duped = (ScreeningPtr) malloc(sizeof(Screening));
    
    duped->hour = curr->hour;
    duped->movie = curr->movie;
    duped->seatsLeft = curr->seatsLeft;
    duped->seats = curr->seats;
    duped->theaterId = curr->theaterId;

    return (void*) duped;
}// Rashman continue from here

// Delete a key from the tree
// O(log(n))
Node* Delete(Node* root, us key) {
    // Root is empty
    if (!root) {
        return root;
    }

    Node* temp;
    us currKey = GetKeyOfNode(root);

    // Go left
    if (key < currKey) {
        root->left = Delete(root->left, key);
    }
    // Go right
    else if (key > currKey) {
        root->right = Delete(root->right, key);
    }
    // Found the one to be deleted
    else {
        if (IsLeaf(root)) {
            free(root->info);
            free(root);
            return NULL;
        }
        else if (!root->left) {
            temp = root->right;
            free(root->info);
            free(root);
            return temp;
        }
        else if (!root->right) {
            temp = root->left;
            free(root->info);
            free(root);
            return temp;
        }
        else {
            temp = root->right;
            while (temp->left) {
                temp = temp->left;
            }
            free(root->info);
            root->info = DupInfo(temp->info);
            root->right = Delete(root->right, GetKeyOfNode(temp));
        }
    }

    // Update height and balance factor
    root->height = 1 + fmax(GetHeight(root->left), GetHeight(root->right));
    int balance = GetBF(root);

    // Make rotations
    if (balance > 1 && GetBF(root->left) >= 0) {
        return LL(root);
    }
    if (balance > 1 && GetBF(root->left) < 0) {
        root->left = RR(root->left);
        return LL(root);
    }
    if (balance < -1 && GetBF(root->right) <= 0) {
        return RR(root);
    }
    if (balance < -1 && GetBF(root->right) > 0) {
        root->right = LL(root->right);
        return RR(root);
    }

    return root;
}

// O(n)
void inorder(Node* root) {
    if (!root) {
        inorder(root->left);
        printf("%d ", ((ScreeningPtr)root->info)->hour);
        inorder(root->right);
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