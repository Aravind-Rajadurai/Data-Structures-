#include "btNode.h"
#include <iostream>
#include <cmath>
using namespace std;

// returns number of nodes in the tree
int  tree_size(btNode* avl_root)
{
    if (avl_root == 0) return 0;
    return 1 + tree_size(avl_root->left) + tree_size(avl_root->right);
}

// returns height of the tree
int height(btNode* root)
{
    if (root == 0) // base case
        return -1;
    int heightLST = height(root->left),
    heightRST = height(root->right);
    return (heightLST > heightRST) ? 1 + heightLST : 1 + heightRST;
}

// return true if AVL tree, otherwise false
bool isAVL(btNode* root)
{
    if(root == 0) return true;
    int heightLST,
    heightRST;
    if (root->right != 0 || root->left != 0)
    {
        heightLST = height(root->left);
        heightRST = height(root->right);
        if ( abs(heightLST - heightRST) > 1 ||
            abs(heightRST - heightLST) > 1)
            return false;
    }
    return isAVL(root->left) && isAVL(root->right);
}

// a non-fancy way to display binary tree
void tree_print(btNode* root, int level)
{
    tree_print_aux(root, root, level);
}

// recursive helper function called by tree_print
void tree_print_aux(btNode* root, btNode* ptr, int level)
{
    if (ptr != 0)
    {
        tree_print_aux(root, ptr->right, level + 1);
        cout << '\n';
        if (ptr == root) cout << "ROOT-=< ";
        for (int i = 0; i < level && ptr != root; ++i)
            cout<<"        ";
        cout << ptr->data;
        tree_print_aux(root, ptr->left, level + 1);
    }
}

// releases dynamically allocated node memory use by
// the tree to heap and set the tree to empty tree
void tree_clear(btNode*& root)
{
    if (root == 0) return;
    tree_clear(root->left);
    tree_clear(root->right);
    delete root;
    root = 0;
}

//////////////////////////////////////////////////////////////////////

// wrapper function that initiates recursive process of avl_insert_aux
// (return value from the initiating call not captured/used)
void avl_insert(btNode*& avl_root, int insInt)
{
    avl_insert_aux(avl_root, insInt);
}

// adds item (with data insInt) to tree rooted at avl_root
// tree is unchanged if a matching item already exists
// return value is the change in depth in the tree
// (for use by ancestor nodes to correct balance factors)
int avl_insert_aux(btNode*& avl_root, int insInt)
{
    // to be filled in as part of AssignOpt01
}

// rebalances an out-of-balance node with taller LST
// does single or double rotation depending on left child's bf
void rebalanceL(btNode*& avl_root)
{
    struct AVLTree *k = root->right;
    
    struct AVLTree *temp = k->left;
    
    k->left = root;
    
    root->right = temp;
    
    root->h = larger(getHeight(root->left), getHeight(root->right))+1;
    
    k->h = larger(getHeight(k->left), getHeight(k->right))+1;
    
    return k;
}

// rebalances an out-of-balance node with taller RST
// does single or double rotation depending on right child's bf
void rebalanceR(btNode*& avl_root)
{
    struct AVLTree *temp = root->left;
    
    struct AVLTree *T = temp->right;
    
    temp->right = root;
    
    root->left = T;
    
    root->h = larger(getHeight(root->left), getHeight(root->right))+1;
    
    temp->h = larger(getHeight(temp->left), getHeight(temp->right))+1;
    
    return temp;
}

// does single left rotation of the tree
// does NOT update bf's (any needed update is to be done @ caller level)
void rotateL(btNode*& avl_root)
{
    // to be filled in as part of AssignOpt01
}

// does single right rotation of the tree
// does NOT update bf's (any needed update is to be done @ caller level)
void rotateR(btNode*& avl_root)
{
    // to be filled in as part of AssignOpt01
}


