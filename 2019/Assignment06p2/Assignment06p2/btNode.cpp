#include "btNode.h"

void dumpToArrayInOrder(btNode* bst_root, int* dumpArray)
{
    if (bst_root == 0) return;
    int dumpIndex = 0;
    dumpToArrayInOrderAux(bst_root, dumpArray, dumpIndex);
}

void dumpToArrayInOrderAux(btNode* bst_root, int* dumpArray, int& dumpIndex)
{
    if (bst_root == 0) return;
    dumpToArrayInOrderAux(bst_root->left, dumpArray, dumpIndex);
    dumpArray[dumpIndex++] = bst_root->data;
    dumpToArrayInOrderAux(bst_root->right, dumpArray, dumpIndex);
}

void tree_clear(btNode*& root)
{
    if (root == 0) return;
    tree_clear(root->left);
    tree_clear(root->right);
    delete root;
    root = 0;
}

int bst_size(btNode* bst_root)
{
    if (bst_root == 0) return 0;
    return 1 + bst_size(bst_root->left) + bst_size(bst_root->right);
}

// write definition for bst_insert here
void bst_insert(btNode*& root, int insInt)
{
    if(root != 0) // Checking for an empty tree
    {
        bool found = false; // Indicator value
        btNode* temp = root; // Copy of root node
        
        do
        {
            if(temp->data > insInt) // Search left tree
            {
                if(temp->left == 0) // Left tree is empty
                {
                    temp->left = new btNode;
                    
                    temp->left->data = insInt;
                    temp->left->left = 0;
                    temp->left->right = 0;
                    
                    found = true;
                }
                else // Left tree not empty, drop down one
                {
                    temp = temp->left;
                }
                
            }
            else if(temp->data < insInt) // Search right tree
            {
                if (temp->right == 0) // Right tree is empty
                {
                    temp->right = new btNode;
                    temp->right->data = insInt;
                    temp->right->left = 0;
                    temp->right->right = 0;
                    
                    found = true;
                }
                else // Right tree not empty, drop down one
                {
                    temp = temp->right;
                }
                
            }
            else // Data = insInt
            {
                temp->data = insInt; // Overwriting as stated in postcondition
                found = true;
            }
        }
        while((temp != 0) && (!found));
        
    }
    else // Empty tree condition
    {
        root = new btNode; // Creating the root
        root->data = insInt; // Assigning node data
        root->left = 0;
        root->right = 0;
    }
    
}


// write definition for bst_remove here
bool bst_remove(btNode*& root, int remInt)
{
    if(root != 0) // Checking for empty tree
    {
        if(root->data == remInt) // The cases from the textbook
        {
            if(root->left != 0) // Root has a left child
            {
                bst_remove_max(root->left, root->data); // Shifting left subtree into root
                return true; // Signalling removal
                
            }
            else // Root has no left child
            {
                btNode* oldRoot = root; // preserving dead root
                root = root->right;
                delete oldRoot; // Freeing original root
                return true; // Signalling the removal
            }
            
        }
        else if(root->data > remInt) // If data > remInt then search in the left tree
        {
            return bst_remove(root->left, remInt);
        }
        else // if data < remInt then search right tree
        {
            return bst_remove(root->right, remInt);
        }
    }
    return false; // Hit the end of the tree return
    
}

// write definition for bst_remove_max here
void bst_remove_max(btNode*& root, int& removed)
{
    if(root != 0) // Checking tree for emptiness
    {
        if(root->right != 0) // If there is a right node
        {
            bst_remove_max(root->right, removed);
        }
        else // I am at the maximum node
        {
            removed = root->data;
            btNode* oldRoot = root; // Preserving dead root
            root = root->left; // Setting the root to the left subtree
            delete oldRoot; // Freeing the max node
        }
        
    }
}
