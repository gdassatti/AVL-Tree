//**********************************//
//  Gavin Dassatti                  //
//  CS 3610                         //
//  Project 02                      //
//                                  //
//  Implementing an AVL Tree        //
//**********************************//





#include <iostream>
#include "avl_tree.h"
using namespace std;

// Insert function that creates the parameters for the insertIntoAVL function
void AVLTree::insert(const int key){
    
    BinaryNode* newNode;
    bool isTaller = false;
    
    newNode = new BinaryNode(key);
    newNode -> bfactor = 0;
    newNode -> left = NULL;
    newNode -> right = NULL;
    insertIntoAVL(root, newNode, isTaller);
    
}

// Parses the input from the user and prints accordingly
void AVLTree::print(char option){
    
    if(root == NULL){
        cout << "EMPTY" << endl;
        return;
    }
    
    if(option == 'p'){
        inOrder(root);
    }else if(option == 'h'){
        print_height(root);
    }
    
}

// User function to remove a node based on a target key value
void AVLTree::remove(int target){
    
    bool isShorter = false;
    
    if(!inTree(root, target)){
        cout << "No node to remove" << endl;
        return;
    }
    
    delete_node(root, target, isShorter);
    
}

// Returns the height for a given node in a AVL Tree
int AVLTree::height(BinaryNode* &root){
    
    if(root == NULL){
        return 0;
    }
    
    int left_height = height(root -> left);
    int right_height = height(root -> right);
    
    if(left_height > right_height){
        
        return (left_height + 1);
        
    }else{
        
        return (right_height + 1);
        
    }
    
}

// In-order tree traversal to print the height of every node
void AVLTree::print_height(BinaryNode* &root){
    
    if(root == NULL){
        return;
    }
    
    print_height(root -> left);
    cout << height(root) << " ";
    print_height(root -> right);
    
}

// In-order tree traversal to print the key value at every node
void AVLTree::inOrder(BinaryNode* &node){
    
    if(node == NULL){
        return;
    }
    inOrder(node -> left);
    cout << node -> key << " ";
    inOrder(node -> right);
}

// Checks if there is a node with the target key value in the tree
bool AVLTree::inTree(BinaryNode* &root, int target){
    
    if(root == NULL){
        
        return false;
        
    }else if(root -> key == target){
        
        return true;
        
    }else{
        
        return (inTree(root -> left, target) || inTree(root -> right, target));
    }
    
}

// Insert function that inserts a new node into the AVL tree and balance it accordingly
void AVLTree::insertIntoAVL(BinaryNode *&root, BinaryNode *newNode, bool &isTaller){
    
    if(root == NULL){
        root = newNode;
        isTaller = true;
    }
    else if(root -> key == newNode -> key){ // checking for duplicate key values
        cout << "DUPLICATE" << endl;
    }
    else if(root -> key > newNode -> key){  // new node goes into the left sub-tree
        insertIntoAVL(root -> left, newNode, isTaller); // Insert new node into left sub-tree
        if(isTaller){                       // If the left sub-tree grew in height after insertion
            switch(root -> bfactor){
                case -1:
                    balanceFromLeft(root);
                    isTaller = false;
                    break;
                case 0:
                    root -> bfactor = -1;
                    isTaller = true;
                    break;
                case 1:
                    root -> bfactor = 0;
                    isTaller = false;
            } // End of switch
    
        } // End of if
        
    }else{
        insertIntoAVL(root -> right, newNode, isTaller); // Insert new node into right sub-tree
        if(isTaller){                       // If the right sub-tree grew in height
            switch(root -> bfactor){
                case -1:
                    root -> bfactor = 0;
                    isTaller = false;
                    break;
                case 0:
                    root -> bfactor = 1;
                    isTaller = true;
                    break;
                case 1:
                    balanceFromRight(root);
                    isTaller = false;
            } // end of switch
        } // end of if
    } // end of else
} // end of insertIntoAVL

// delete_node function deletes a node in the AVL tree given a target key value and balances it accordingly
void AVLTree::delete_node(BinaryNode* &root, int target, bool& isShorter){
    
    if(root == NULL){
        return;
    }
    if(target < root -> key){ // If the target is in the left sub-tree
        delete_node(root -> left, target, isShorter);   // Calling function
        if(isShorter){
            switch(root -> bfactor){
                case -1:        // Left heavy case, now even
                    root -> bfactor = 0;
                    isShorter = true;
                    break;
                case 0:         // Even case, now right heavy
                    root -> bfactor = 1;
                    isShorter = false;
                    break;
                case 1:         // Right heavy case, now needs balanced
                    BinaryNode* node = root -> right;
                    switch(node -> bfactor){
                        case -1:    // Right-left rebalance
                            rotateToRight(node);
                            root -> right = node;
                            rotateToLeft(root);
                            isShorter = true;
                            root -> bfactor = height(root -> right) - height(root -> left);
                            node -> bfactor = height(node -> right) - height(node -> left);
                            break;
                        case 0:     // Normal rebalance
                            rotateToLeft(root);
                            root -> bfactor = height(root -> right) - height(root -> left);
                            node -> bfactor = height(node -> right) - height(node -> left);
                            isShorter = false;
                            break;
                        case 1:     // Right-right rebalance
                            rotateToLeft(root);
                            root -> bfactor = height(root -> right) - height(root -> left);
                            node -> bfactor = height(node -> right) - height(node -> left);
                            isShorter = true;
                            break;
                    } // End of inner switch
                break;
            } // End of outer switch
        } // end of if
    }else if(target > root -> key){ // If the target is in the right sub-tree
        delete_node(root -> right, target, isShorter);  // Calling function
        if(isShorter){
            switch (root -> bfactor) {
                case 1:         // Right heavy case, now even
                    root -> bfactor = 0;
                    isShorter = true;
                    break;
                case 0:         // Even case, now left heavy
                    root -> bfactor = -1;
                    isShorter = false;
                    break;
                case -1:        // Left heavy case, now needs rebalanced
                    BinaryNode* node = root -> left;
                    switch(node -> bfactor){
                        case 1:     // Left-right rebalance
                            rotateToLeft(node);
                            root -> left = node;
                            rotateToRight(root);
                            isShorter = true;
                            root -> bfactor = height(root -> right) - height(root -> left);
                            node -> bfactor = height(node -> right) - height(node -> left);
                            break;
                        case 0:     // Normal rebalance
                            rotateToRight(root);
                            root -> bfactor = height(root -> right) - height(root -> left);
                            node -> bfactor = height(node -> right) - height(node -> left);
                            isShorter = false;
                            break;
                        case -1:    // Left-left rebalance
                            rotateToRight(root);
                            root -> bfactor = height(root -> right) - height(root -> left);
                            node -> bfactor = height(node -> right) - height(node -> left);
                            isShorter = true;
                            break;
                    } // End of inner switch
                break;
            } // End of outer switch
        } // End right sub-tree
    }else{
        if((root -> left == NULL) || (root -> right = NULL)){ // If the target is the root
            BinaryNode* node;   // One or both roots are null
            
            if(root -> left == NULL){
                node = root -> right;
            }else{
                node = root -> left;
            }
            // Leaf case
            if(node == NULL){
                node = root;
                root = NULL;
            }else{              // One child case
                *root = *node;  // Copying root to node
            }
            
            delete node;
            isShorter = true;
            
        }else{                  // two child case
            BinaryNode* node = predecessor(root -> right); // Getting a new value in the right subtree
            root -> key = node -> key;
            delete_node(root -> right, node -> key, isShorter);
        }
    }
}

// Predecessor function returns next node for in-Order traversal from root
BinaryNode* AVLTree::predecessor(BinaryNode* &root){
    
    BinaryNode* current = root;
    while(current -> left != NULL){
        current = current -> left;
    }
    return current;
    
}

// Balance decision-making for inserting a new node into a left-heavy tree
void AVLTree::balanceFromLeft(BinaryNode* &root){
    
    BinaryNode* p;
    BinaryNode* w;
    p = root -> left;   // p points to left subtree of root
    switch(p -> bfactor){
        case -1:
            root -> bfactor = 0;
            p -> bfactor = 0;
            rotateToRight(root);
            break;
        case 0:
            cerr << "Error: Cannot balance from the left" << endl;
            break;
        case 1:
            w = p -> right;
            switch(w -> bfactor){   // adjusting the balance factors
                case -1:
                    root -> bfactor = 1;
                    p -> bfactor = 0;
                    break;
                case 0:
                    root -> bfactor = 0;
                    p -> bfactor = 0;
                    break;
                case 1:
                    root -> bfactor = 0;
                    p -> bfactor = -1;
            } // end of inner switch
        w -> bfactor = 0;
        rotateToLeft(p);
        root -> left = p;
        rotateToRight(root);
    } // end of inner switch
} // End of balanceFromLeft

// Balance decision-making for inserting a new node into a right-heavy tree
void AVLTree::balanceFromRight(BinaryNode* &root){
    
    BinaryNode* p;
    BinaryNode* w;
    p = root -> right; // p points to right subtree of root
    switch(p -> bfactor){
        case -1:
            w = p -> left;
            switch(w -> bfactor){   // adjusting the balance factors
                case -1:
                    root -> bfactor = 0;
                    p -> bfactor = 1;
                    break;
                case 0:
                    root -> bfactor = 0;
                    p -> bfactor = 0;
                    break;
                case 1:
                    root -> bfactor = -1;
                    p -> bfactor = 0;
            } //  end of inner switch
            w -> bfactor = 0;
            rotateToRight(p);
            root -> right = p;
            rotateToLeft(root);
            break;
        case 0:
            cerr << "Error: cannot balance from the right" << endl;
            break;
        case 1:
            root -> bfactor = 0;
            p -> bfactor = 0;
            rotateToLeft(root);
    } // end of inner switch
} // end of balance from right
    
// Left rotation function
void AVLTree::rotateToLeft(BinaryNode* &root){
    
    BinaryNode* p;
    if(root == NULL){
        cerr << "Error in the tree" << endl;
    }else if(root -> right == NULL){
        cerr << "Error in the tree: No right subtree to rotate" << endl;
    }else{
        p = root -> right;          // p points to right subtree of root
        root -> right = p -> left;  // left subtree of p becomes
        p -> left = root;           // right subtree of root
        root = p;
    }   // end of if
    
}   // end of rotateToLeft

// Right rotation
void AVLTree::rotateToRight(BinaryNode* &root){
    
    BinaryNode* p;
    if(root == NULL){
        cerr << "Error in the tree" << endl;
    }else if(root -> left == NULL){
        cerr << "Error in the tree: No left subtree to rotate" << endl;
    }else{
        p = root -> left;           // p points to left subtree of root
        root -> left = p -> right;  // right subtee of p becomes
        p -> right = root;          // left subtree of root
        root = p;
    }   // end of if
}   // end of rotateToRight








