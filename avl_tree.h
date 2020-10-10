#ifndef AVL_TREE_H
#define AVL_TREE_H

struct BinaryNode {
    
  BinaryNode(const int key) : key(key), left(NULL), right(NULL) {}

  ~BinaryNode() {
    delete left;
    delete right;
    left = right = NULL;
  }

  int key, bfactor;
  BinaryNode *left, *right;
    
};


class AVLTree {

public:
    
    // PUBLIC MEMBER FUNCTION
    
    AVLTree() : root(NULL) {}
    ~AVLTree() {
        delete root;
    }

    void insert(const int key);
    void print(char option);
    void remove(int target);

private:
    
    // PRIVATE MEMBER FUNCTION
    
    int height(BinaryNode* &root);
    void print_height(BinaryNode* &root);
    void inOrder(BinaryNode* &node);
    bool inTree(BinaryNode* &root, int target);
    void insertIntoAVL(BinaryNode* &root, BinaryNode* newNode, bool& isTaller);
    void delete_node(BinaryNode* &root, int target, bool& isShorter);
    BinaryNode* predecessor(BinaryNode* &root);
    void balanceFromLeft(BinaryNode* &root);
    void balanceFromRight(BinaryNode* &root);
    void rotateToLeft(BinaryNode* &root);
    void rotateToRight(BinaryNode* &root);
    
    BinaryNode* root;
    
};

#endif
