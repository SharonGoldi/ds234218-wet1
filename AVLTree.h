
#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

#include <stdexcept>
#include <cassert>

typedef enum {
    TREE_SUCCESS = 0,
    TREE_FAILURE = -1,
    TREE_ALLOCATION_ERROR = -2,
    TREE_INVALID_INPUT = -3
} TreeStatusType;

static int max(int a, int b) {
    if (a>b) {
        return a;
    }
    return b;
}

template <class KeyType, class ValueType>
struct Node{
    KeyType* key;
    ValueType* value;
    Node*  parent;
    Node*  left;
    Node*  right;
    int height;
};

template <class KeyType, class ValueType>
class AvlTree{
    // root is a dummy, the real tree root is its right son
    Node<KeyType,ValueType>* root;
    int sum;

    /**
     * balances a tree when given the problematic node
     * @param tempParent - the node with the problematic balance factor
     */
    void BalanceTree(Node<KeyType,ValueType>* tempParent);
    /**
     * calcs the balance factor of a node
     * @param root - the node calcolated
     */
    int BF(const Node<KeyType,ValueType>* root) const;
    /**
     * makes a LL roll
     * @param root - the node with the bad balance factor that caused the roll
     */
    void LLRoll(Node<KeyType,ValueType>* root);
    /**
     * makes a LR roll
     * @param root - the node with the bad balance factor that caused the roll
     */
    void LRRoll(Node<KeyType,ValueType>* root);
    /**
     * makes a RL roll
     * @param root - the node with the bad balance factor that caused the roll
     */
    void RLRoll(Node<KeyType,ValueType>* root);
    /**
     * makes the RR roll
     * @param root - the node with the bad balance factor that caused the roll
     */
    void RRRoll(Node<KeyType,ValueType>* root);
    /**
   * swaps between two nodes
   * @param a - node a, higher on the tree than b
   * @param b - node b
   */
    void Swap(Node<KeyType,ValueType>* a, Node<KeyType,ValueType>* b);
    /**
     * updates the height of a specific node
     * @param node - the node calcolated
     */
    void UpdateHeight(Node<KeyType,ValueType>* node);
    /**
     * puts the node pointers in an array inorder
     * @param array - an empty array of Nodes, in the size of the tree size
     * (without the dummy)
     * @param node - a pointer (Node) to the root of the tree (not dummy)
     * @param counter - a recursive param, needs to point to 0 when we call the func
     */
    void InOrder(Node<KeyType,ValueType>** array,
                 Node<KeyType,ValueType>* node, int* counter);
    /**
     *
     * @param array - an empty array of Nodes, in the size of the tree size
     * (without the dummy)
     * @param node - a pointer (Node) to the root of the tree (not dummy)
     * @param counter - a recursive param, needs to point to 0 when we call the func
     */
    void PreOrder(Node<KeyType,ValueType>** array,
                  Node<KeyType,ValueType>* node, int* counter);
    /**
     * deletes all the nodes of a tree except the dummy
     * @param node - a pointer to the root of the tree
     */
    void DeleteTreesNodes(Node<KeyType,ValueType>* node);
    /**
     * copies the nodes of a tree, without the dummy
     * @param copyToParent - a pointer to the copied parent of the next copied
     * node
     * @param copyTo - a pointer to the place of the next copied node to be in
     * @param InOrder - an array of the Nodes of the copied tree organized
     * inorder
     * @param PreOrder - an array of the Nodes of the copied tree organized
     * preorder
     * @param treeSize - the number of nodes in that tree
     */
    void CopyAux(Node<KeyType,ValueType>* copyToParent,
                 Node<KeyType,ValueType>* copyTo,
                 Node<KeyType,ValueType>** InOrder,
                 Node<KeyType,ValueType>** PreOrder, int treeSize);
    /**
     * fills an empty tree with a copy of another tree
     * @param toCopy - a pointer to the tree that will be copied
     * @param copyTo - a pointer to the tree that will be filled
     */
    void Copy(AvlTree<KeyType,ValueType> const& toCopy,
              AvlTree<KeyType,ValueType>& copyTo);
    /**
     * adds a given node to the Avl tree in the right place and updates the
     * trees parameters
     * @param toPlace - a node that will be placed in the tree
     * @return TREE_SUCCESS - if the new node was placed successfully
     *         TREE_FAILURE - otherwise (e.g. the key already exists).
     */
    TreeStatusType PlaceNode(Node<KeyType,ValueType>* toPlace);
    /**
     * finds if there is a node with the given key in the tree
     * @param key - a key to look for in the tree
     * @return a pointer to the node with the same key in the tree, or NULL if
     * there is no node found
     */
    Node<KeyType,ValueType>* FindNodeByKey(KeyType const& key) const;
    /**
     * removes the root from the tree if its the only node in it
     */
    void RootRemove();
    /**
     * removes the leaf from the tree
     * @param toRemove - a node to remove, the node is a leaf
     */
    void LeafRemove(Node<KeyType,ValueType>* toRemove);
    /**
     * removes the node from the tree
     * @param toRemove - a node to remove, the node has only one son
     */
    void OneChildRemove(Node<KeyType,ValueType>* toRemove);
    /**
     * removes the node from the tree
     * @param toRemove - a node to remove, the node has 2 sons
     */
    void TwoChildRemove(Node<KeyType,ValueType>* toRemove);
    /**
     * finds the next node inorder from the tree
     * @param node - a node to find its next inorder
     * @return - a pointer to the next node
     */
    Node<KeyType,ValueType>* GetNextInOrder(Node<KeyType,ValueType>* node);

public:
    AvlTree<KeyType,ValueType>();
    AvlTree<KeyType,ValueType>(AvlTree<KeyType,ValueType>& tree);
    ~AvlTree();
    AvlTree& operator=(AvlTree<KeyType,ValueType> const& tree);

    TreeStatusType Add(KeyType& key, ValueType& value);
    TreeStatusType Find(const KeyType& key, ValueType* value) const;
    TreeStatusType Delete(const KeyType& key);
    TreeStatusType DeleteByPointer(Node<KeyType,ValueType>* toDelete);
    TreeStatusType Size(int* n) const;

    void GetTreeInOrder(Node<KeyType,ValueType>** const array);

};
/****************************************************************************/

// tree balancing funcs
template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::BalanceTree(Node<KeyType,ValueType>* tempParent) {
    int bf = this->BF(tempParent);
    if (bf == 2) {
        if (this->BF(tempParent->left) == -1) {
            this->LRRoll(tempParent);
        }
        else {
            this->LLRoll(tempParent);
        }
    }
    if (bf == -2) {
        if (this->BF(tempParent->right) == 1) {
            this->RLRoll(tempParent);
        }
        else {
            this->RRRoll(tempParent);
        }
    }
}

template <class KeyType, class ValueType>
int AvlTree<KeyType,ValueType>::BF(const Node<KeyType,ValueType>* const root) const {
    assert (root != NULL);
    if(root->right == NULL && root->left == NULL) {
        return 0;
    }
    if (root->right == NULL) {
        return root->left->height + 1;
    }
    else if (root->left == NULL) {
        return -1 - root->right->height;
    }
    return root->left->height - root->right->height;
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::LLRoll(Node<KeyType,ValueType>* root) {
    Node<KeyType,ValueType>* B = root;
    Node<KeyType,ValueType>* A = root->left;

    if (B->parent == NULL) {
        this->root->right = A;
    } else {
        bool isRight = (B == B->parent->right);
        if (isRight) {
            B->parent->right = A;
        } else {
            B->parent->left = A;
        }
    }

    A->parent = B->parent;
    B->parent = A;

    B->left = A->right;
    if (A->right != NULL) {
        A->right->parent = B;
    }
    A->right = B;

    UpdateHeight(B);
    UpdateHeight(A);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::RRRoll(Node<KeyType,ValueType>* const root) {
    Node<KeyType,ValueType>* B = root;
    Node<KeyType,ValueType>* A = root->right;

    if (B->parent == NULL) {
        this->root->right = A;
    } else {
        bool isRight = (B == B->parent->right);
        if (isRight) {
            B->parent->right = A;
        } else {
            B->parent->left = A;
        }
    }

    A->parent = B->parent;
    B->parent = A;


    B->right = A->left;
    if (A->left != NULL) {
        A->left->parent = B;
    }
    A->left = B;

    UpdateHeight(B);
    UpdateHeight(A);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::LRRoll(Node<KeyType,ValueType>* const root) {
    RRRoll(root->left);
    LLRoll(root);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::RLRoll(Node<KeyType,ValueType>* const root) {
    LLRoll(root->right);
    RRRoll(root);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::Swap(Node<KeyType,ValueType>* const a, Node<KeyType,ValueType>* const b) {
    assert( a != NULL && b != NULL);

    if (!a->parent) { // root is always a right son of the dummy
        this->root->right = b;
    } else if (a->parent->right == a){
        a->parent->right = b;
    } else {
        a->parent->left = b;
    }

    if (b->parent == a) {
        b->parent = a->parent;
        a->parent = b;

        a->right = b->right;
        b->right = a;

        b->left = a->left;
        a->left = NULL;

        int tempH = a->height;
        a->height = b->height;
        b->height = tempH;

        b->left->parent = b;

        if (a->right) {
            a->right->parent = a;
        }
    } else {
        //swap the nodes sons and ptr to parent
        Node<KeyType,ValueType>* tempParent = a->parent;
        Node<KeyType,ValueType>* tempRight = a->right;
        Node<KeyType,ValueType>* tempLeft = a->left;
        int tempH = a->height;

        a->parent = b->parent;
        a->height = b->height;
        a->right = b->right;
        a->left = b->left;

        b->parent = tempParent;
        b->height = tempH;
        b->right = tempRight;
        b->left = tempLeft;

        b->right->parent = b;
        b->left->parent = b;

        if (a->right) {
            a->right->parent = a;
        }
    }
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::UpdateHeight(Node<KeyType,ValueType>* const node) {
    assert(node != NULL);
    if (node->right == NULL && node->left == NULL) {
        node->height = 0;
    }
    else if (node->right == NULL) {
        node->height = 1 + node->left->height;
    }
    else if (node->left == NULL) {
        node->height = 1 + node->right->height;
    } else {
        node->height = 1 + max(node->right->height,node->left->height);
    }
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::GetTreeInOrder(
        Node<KeyType, ValueType>** const array) {

    int counter = 0;
    InOrder(array, this->root->right, &counter);
}


template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::InOrder(Node<KeyType,ValueType>** const array,
                                         Node<KeyType,ValueType>* const node,
                                         int* counter) {
    if (node == NULL) {
        return;
    }
    InOrder(array, node->left, counter);
    array[*counter] = node;
    (*counter)++;
    InOrder(array, node->right, counter);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::PreOrder(Node<KeyType,ValueType>** array,
                                          Node<KeyType,ValueType>* const node,
                                          int* counter) {
    if (node == NULL) {
        return;
    }
    array[*counter] = node;
    (*counter)++;
    PreOrder(array, node->left, counter);
    PreOrder(array, node->right, counter);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::DeleteTreesNodes(Node<KeyType,ValueType>* node) {
    if (node == NULL) {
        return;
    }
    DeleteTreesNodes(node->left);
    DeleteTreesNodes(node->right);
    delete node->value;
    delete node->key;
    delete node;
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::CopyAux(Node<KeyType,ValueType>* copyToParent,
                                         Node<KeyType,ValueType>* copyTo,
                                         Node<KeyType,ValueType>** const InOrder,
                                         Node<KeyType,ValueType>** const PreOrder,
                                         int treeSize) {
    if(treeSize == 0) {
        return;
    }

    copyTo = new Node<KeyType,ValueType>;
    copyTo->parent = copyToParent;
    copyTo->key = new KeyType(*(PreOrder[0]->key));
    copyTo->value = new ValueType(*(PreOrder[0]->value));
    copyTo->height = PreOrder[0]->height;

    int index = -1;
    for (int i = 0; i < treeSize; i++) {
        if(PreOrder[0]->key == InOrder[i]->key) {
            index = i;
            break;
        }
    }

    CopyAux(copyTo, copyTo->left, InOrder, PreOrder+1,index);
    CopyAux(copyTo, copyTo->right, InOrder+index+1, PreOrder+index+1,
            treeSize-index-1);
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::Copy(AvlTree<KeyType,ValueType> const& toCopy,
                                      AvlTree<KeyType,ValueType>& copyTo) {
    Node<KeyType,ValueType>** treeInOrder = new Node<KeyType,ValueType>* [toCopy.sum];
    int in_counter = 0;
    InOrder(treeInOrder, toCopy.root->right, &in_counter);

    int pre_counter = 0;
    Node<KeyType,ValueType>** treePreOrder = new Node<KeyType,ValueType>* [toCopy.sum];
    PreOrder(treePreOrder, toCopy.root->right, &pre_counter);

    CopyAux(copyTo.root, copyTo.root->right, treeInOrder, treePreOrder, toCopy.sum);

    delete[] treeInOrder;
    delete[] treePreOrder;
}

template <class KeyType, class ValueType>
TreeStatusType AvlTree<KeyType,ValueType>::PlaceNode(Node<KeyType,ValueType>* toPlace) {
    //if the tree is empty
    if (this->sum == 0) {
        this->sum++;
        toPlace->parent = NULL;
        this->root->right = toPlace;
        return TREE_SUCCESS;
    }
    //else find nodes place
    Node<KeyType,ValueType>* tempParent = this->root;
    Node<KeyType,ValueType>* temp = this->root->right;

    while (temp != NULL) {
        if(*(temp->key) == *(toPlace->key)) {
            return TREE_FAILURE;
        }
        else if (*(temp->key) < *(toPlace->key)) {
            tempParent = temp;
            temp = temp->right;
        }
        else if (*(temp->key) > *(toPlace->key)) {
            tempParent = temp;
            temp = temp->left;
        }
    }
    //place node
    //a new node is always inserted as a leaf (before balancing)
    toPlace->parent = tempParent;
    this->sum++;
    if (*(tempParent->key) < *(toPlace->key)) {
        tempParent->right = toPlace;
    }
    else if (*(tempParent->key) > *(toPlace->key)) {
        tempParent->left = toPlace;
    }

    while (tempParent != NULL) {
        UpdateHeight(tempParent);
        BalanceTree(tempParent);
        tempParent = tempParent->parent;
    }
    return TREE_SUCCESS;
}

template <class KeyType, class ValueType>
Node<KeyType,ValueType>* AvlTree<KeyType,ValueType>::FindNodeByKey(KeyType const& key) const {
    Node<KeyType,ValueType>* temp = this->root->right;

    while (temp != NULL) {
        if (*(temp->key) == key) {
            return temp;
        }
        else if (*(temp->key) < key) {
            temp = temp->right;
        }
        else if (*(temp->key) > key) {
            temp = temp->left;
        }
    }
    return NULL;
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::RootRemove() {
    // if toDelete is the only node in the tree, the tree will be empty.
    this->root->right = NULL;
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::LeafRemove(Node<KeyType,ValueType>* toRemove) {
    assert (toRemove != NULL);
    bool isRightSon = (toRemove == toRemove->parent->right);
    if (isRightSon) {
        toRemove->parent->right = NULL;
    } else {
        toRemove->parent->left = NULL;
    }
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::OneChildRemove(Node<KeyType,ValueType>* toRemove) {
    assert (toRemove != NULL);
    bool isRoot = toRemove->parent == NULL;
    bool isRightSon = !isRoot && toRemove == toRemove->parent->right;

    bool hasRightSon = toRemove->right != NULL;

    if (isRoot) {
        if (hasRightSon) {
            this->root->right = toRemove->right;
            toRemove->right->parent = NULL;
        } else {
            this->root->right = toRemove->left;
            toRemove->left->parent = NULL;
        }
    } else if(isRightSon) {
        if (hasRightSon) {
            toRemove->parent->right = toRemove->right;
            toRemove->right->parent = toRemove->parent;
        } else {
            toRemove->parent->right = toRemove->left;
            toRemove->left->parent = toRemove->parent;
        }
    } else {
        if (hasRightSon) {
            toRemove->parent->left = toRemove->right;
            toRemove->right->parent = toRemove->parent;
        } else {
            toRemove->parent->left = toRemove->left;
            toRemove->left->parent = toRemove->parent;
        }
    }
}

template <class KeyType, class ValueType>
void AvlTree<KeyType,ValueType>::TwoChildRemove(Node<KeyType,ValueType>* toRemove) {
    assert (toRemove != NULL);
    Node<KeyType, ValueType> *next = GetNextInOrder(toRemove->right);
    Swap(toRemove, next);

    if (toRemove->right == NULL && toRemove->left == NULL) {
        LeafRemove(toRemove);
    } else {
        OneChildRemove(toRemove);
    }
}

template <class KeyType, class ValueType>
Node<KeyType, ValueType>* AvlTree<KeyType,ValueType>::GetNextInOrder(Node<KeyType, ValueType> *node) {
    assert(node != NULL);
    if(node->left != NULL) {
        return GetNextInOrder(node->left);
    } else {
        return node;
    }
}

// AvlTree basic funcs
template <class KeyType, class ValueType>
AvlTree<KeyType,ValueType>::AvlTree(){
    this->root = new Node<KeyType,ValueType>;
    this->root->key = NULL;
    this->root->right = NULL;
    this->root->left = NULL;
    this->root->parent = NULL;
    this->root->height = -1;
    this->sum = 0;
}

template <class KeyType, class ValueType>
AvlTree<KeyType,ValueType>::AvlTree(AvlTree<KeyType,ValueType>& tree) {

    this->sum = tree.sum;
    this->root->left = NULL;
    Copy(tree,*this);
}

template <class KeyType, class ValueType>
AvlTree<KeyType,ValueType>& AvlTree<KeyType,ValueType>::operator=(AvlTree<KeyType,
        ValueType> const& tree){
    if (this == &tree) {
        return *this;
    }

    //delete the old tree
    Node<KeyType,ValueType>** thisInOrder = new Node<KeyType,ValueType>* [this->sum];
    GetTreeInOrder(thisInOrder);
    for (int i = 0; i < this->sum; i++) {
        delete thisInOrder[i]->key;
        delete thisInOrder[i]->value;
        delete thisInOrder[i];
    }
    delete[] thisInOrder;

    //copy the new tree
    this->sum = tree.sum;
    this->root->left = NULL;
    Copy(tree,*this);

    return *this;
}

template <class KeyType, class ValueType>
AvlTree<KeyType,ValueType>::~AvlTree() {
    DeleteTreesNodes(this->root->right);
    delete this->root;
}


//AvlTree funcs
template <class KeyType, class ValueType>
TreeStatusType AvlTree<KeyType,ValueType>::Add(KeyType& key, ValueType& value) {
//    if (node == NULL) {
//        return TREE_INVALID_INPUT;
//    }

    //build the node
    Node<KeyType,ValueType>* newNode;
    try {
        newNode = new Node<KeyType,ValueType>;
    }
    catch (std::bad_alloc &ba) {
        return TREE_ALLOCATION_ERROR;
    }

    //fill the parameters
    newNode->height = 0;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->key = new KeyType(key);
    newNode->value = new ValueType(value);

    //place the node
    if (PlaceNode(newNode) == TREE_FAILURE) {
        //wont happen in the tests, for the code
        delete newNode->key;
        delete newNode->value;
        delete newNode;
        newNode = NULL;

//        *node = NULL;
        return TREE_FAILURE;
    }
//    *node = newNode;  // TODO: delete node from function call
    return TREE_SUCCESS;
}

template <class KeyType, class ValueType>
TreeStatusType AvlTree<KeyType,ValueType>::Find(const KeyType& key,
                                                ValueType* value) const {
    if (value == NULL) {
        return TREE_INVALID_INPUT;
    }
    Node<KeyType,ValueType>* found = FindNodeByKey(key);

    if (found == NULL) {
        return TREE_FAILURE;
    } else {
        *value = *(found->value);
        return TREE_SUCCESS;
    }
}

template <class KeyType, class ValueType>
TreeStatusType AvlTree<KeyType,ValueType>::Delete(const KeyType& key) {
    Node<KeyType,ValueType>* toDelete = FindNodeByKey(key);
    if (toDelete == NULL) {
        return TREE_FAILURE;
    }
    return DeleteByPointer(toDelete);
}

template <class KeyType, class ValueType>
TreeStatusType AvlTree<KeyType,ValueType>::DeleteByPointer(Node<KeyType,ValueType>*
toDelete) {
    if (toDelete == NULL) {
        return TREE_INVALID_INPUT;
    }
    //remove node from the tree
    if (this->sum == 1) {
        RootRemove();
    } else if (toDelete->right == NULL && toDelete->left == NULL) {
        LeafRemove(toDelete);
    } else if (toDelete->right == NULL || toDelete->left == NULL) {
        OneChildRemove(toDelete);
    } else {
        TwoChildRemove(toDelete);
    }

    //update heights and check BF for the parents
    Node<KeyType, ValueType> *temp = toDelete->parent;
    while (temp != NULL) {
        UpdateHeight(temp);
        BalanceTree(temp);
        temp = temp->parent;
    }
    //delete the nodes info
    delete toDelete->value;
    delete toDelete->key;
    delete toDelete;
    this->sum--;
    return TREE_SUCCESS;
}

template <class KeyType, class ValueType>
TreeStatusType AvlTree<KeyType,ValueType>::Size(int* n) const {
    if (n == NULL) {
        return TREE_INVALID_INPUT;
    }

    *n = this->sum;
    return TREE_SUCCESS;
}


#endif //WET1_AVLTREE_H
