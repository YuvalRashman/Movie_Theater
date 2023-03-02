// Avl handler
#include <stdio.h>
#include <stdlib.h>
#include <Math.h>

typedef short Bool;
typedef unsigned short us;
typedef struct node {
    void* info;
    us height;
    struct node* left;
    struct node* right;
} Node;

// Create a new node with the given key
// O(1)
Node* NewNode(void* info) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->info = info;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Get the height of the node
// O(1)
us GetHeight(Node* root) {
    if (!root) {
        return 0;
    }

    return root->height;
}

// Get the balance factor of the node
// O(1)
us GetBF(Node* root) {
    if (!root) {
        return 0;
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

// O(1)
// Info of node is void* so the method extracts its key!
us GetKeyOfNode(Node* root)
{
    return ((ScreeningPtr)root->info)->hour;
}

// Insert a key into the tree
// O(log(n))
Node* Insert(Node* root, void* info) {
    us currBF;
    us currKey = GetKeyOfNode(root);
    us newKey = ((ScreeningPtr)info)->hour;

    // Root is empty
    if (!root) {
        return NewNode(info);
    }

    // Go left
    if (newKey < currKey) {
        root->left = Insert(root->left, info);
    }
    // Go right
    else {
        root->right = Insert(root->right, info);
    }

    // update height and the balance factor of the current node
    root->height = 1 + fmax(GetHeight(root->left), GetHeight(root->right));
    currBF = GetBF(root);

    // Perform rotations
    // LL
    if (currBF > 1 && newKey < GetKeyOfNode(root->left)) {
        return LL(root);
    }
    // RR
    else if (currBF < -1 && newKey > GetKeyOfNode(root->right)) {
        return RR(root);
    }
    // LR
    else if (currBF > 1 && newKey > GetKeyOfNode(root->left)) {
        root->left = RR(root->left);
        return LL(root);
    }
    // RL
    else (currBF < -1 && newKey < GetKeyOfNode(root->right)) {
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
        us currKey = GetKeyOfNode(root);

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
    ScreeningPtr duped = (ScreeningPtr)malloc(sizeof(Screening));
    
    duped->hour = curr->hour;
    duped->movieId = curr->movieId;
    duped->seatsLeft = curr->seatsLeft;
    duped->seats = curr->seats;

    return (void*) duped;
}

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

// O(1)
Bool IsLeaf(Node* root)
{
    return !root->left & !root->right;
}

// O(n)
void inorder(Node* root) {
    if (!root) {
        inorder(root->left);
        printf("%d ", root->key);
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