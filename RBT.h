// {
//     CREATED BY: CHINTAN ACHARYA
//     DATE: 5 APRIL 2023
// }

#include <iostream>

using namespace std;

// DATA STRUCTURE THAT REPRESENTS A NODE IN THE TREE
struct Node
{
    int rideNumber, rideCost, tripDuration; // DATA TRIPLET
    Node *parent;                           // POINTER TO THE PARENT
    Node *left;                             // POINTER TO THE LEFT CHILD
    Node *right;                            // POINTER TO THE RIGHT CHILD
    int color;                              // 1 -> RED, 0 -> BLACK
};

typedef Node *NodePtr;

// IMPLEMENTS THE OPERATIONS OF RED-BLACK TREE
class RBTree
{
private:
    NodePtr root;
    NodePtr TNULL;

    // INTIALIZES THE NODE WITH APPROPRIATE VALUES
    // ALL THE POINTERS ARE SET TO POINT TO THE NULL POINTER
    void initializeNULLNode(NodePtr node, NodePtr parent)
    {
        node->rideNumber = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
    }

    NodePtr searchTreeHelper(NodePtr node, int key)
    {
        if (node == TNULL || key == node->rideNumber)
        {
            return node;
        }

        if (key < node->rideNumber)
        {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // FIX THE RB TREE MODIFIED BY THE DELETE OPERATION
    void fixDelete(NodePtr x)
    {
        NodePtr s;
        while (x != root && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s->color == 1)
                {
                    // CASE 1
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0)
                {
                    // CASE 2
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->right->color == 0)
                    {
                        // CASE 3
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    // CASE 4
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    // CASE 1
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0)
                {
                    // CASE 2
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == 0)
                    {
                        // CASE 3
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    // CASE 4
                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void rbTransplant(NodePtr u, NodePtr v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    //  INORDER TRAVERSAL FOR RANGE SEARCH
    void inOrderHelper(NodePtr node, int k1, int k2, vector<NodePtr> &result)
    {
        vector<NodePtr> nodes;
        if (node == nullptr)
        {
            return;
        }
        if (node->rideNumber > k1)
        {
            inOrderHelper(node->left, k1, k2, result);
        }
        if (node->rideNumber >= k1 && node->rideNumber <= k2)
        {
            result.push_back(node);
        }
        if (node->rideNumber < k2)
        {
            inOrderHelper(node->right, k1, k2, result);
        }
    }

    // DELETE NODE
    void deleteNodeHelper(NodePtr node, int key)
    {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL)
        {
            if (node->rideNumber == key)
            {
                z = node;
            }

            if (node->rideNumber <= key)
            {
                node = node->right;
            }
            else
            {
                node = node->left;
            }
        }

        if (z == TNULL)
        {
            cout << "Couldn't find key in the tree" << endl;
            return;
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL)
        {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == TNULL)
        {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0)
        {
            fixDelete(x);
        }
    }

    // FIXING RED BLACK TREE
    void fixInsert(NodePtr k)
    {
        NodePtr u;
        while (k->parent->color == 1)
        {
            if (k->parent == k->parent->parent->right)
            {
                u = k->parent->parent->left;
                if (u->color == 1)
                {
                    // CASE 1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)
                    {
                        // CASE 2
                        k = k->parent;
                        rightRotate(k);
                    }
                    // CASE 2
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            }
            else
            {
                u = k->parent->parent->right;

                if (u->color == 1)
                {
                    // MIRROR CASE 1
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        // MIRROR CASE 2
                        k = k->parent;
                        leftRotate(k);
                    }
                    // MIRROR CASE 2
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root)
            {
                break;
            }
        }
        root->color = 0;
    }

public:
    RBTree()
    {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    // SEARCH THE TREE FOR THE KEY K
    // AND RETURN THE CORRESPONDING NODE
    NodePtr searchTree(int k)
    {
        return searchTreeHelper(this->root, k);
    }

    // FIND THE NODE WITH MINIMUM KEY
    NodePtr minimum(NodePtr node)
    {
        while (node->left != TNULL)
        {
            node = node->left;
        }
        return node;
    }

    // FIND THE NODE WITH MAXIMUM KEY
    NodePtr maximum(NodePtr node)
    {
        while (node->right != TNULL)
        {
            node = node->right;
        }
        return node;
    }

    // FIND THE SUCCESSOR FOR THE NODE
    NodePtr successor(NodePtr x)
    {
        if (x->right != TNULL)
        {
            return minimum(x->right);
        }
        NodePtr y = x->parent;
        while (y != TNULL && x == y->right)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }

    // FIND THE PREDECESSOR FOR THE NODE
    NodePtr predecessor(NodePtr x)
    {
        if (x->left != TNULL)
        {
            return maximum(x->left);
        }

        NodePtr y = x->parent;
        while (y != TNULL && x == y->left)
        {
            x = y;
            y = y->parent;
        }

        return y;
    }

    // LEFT ROTATE NODE
    void leftRotate(NodePtr x)
    {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // RIGHT ROTATE NODE
    void rightRotate(NodePtr x)
    {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    vector<NodePtr> rangeSearch(int k1, int k2)
    {
        vector<NodePtr> result;
        inOrderHelper(root, k1, k2, result);
        return result;
    }

    // INSERT THR KEY TO THE TREE IN ITS CORRECT POSITION
    // AND FIX THE TREE
    NodePtr insert(int rideNumber, int rideCost, int tripDuration)
    {
        NodePtr node = new Node;
        node->parent = nullptr;
        node->rideNumber = rideNumber;
        node->rideCost = rideCost;
        node->tripDuration = tripDuration;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1; // NEW NODE IS ALWAYS RED

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != TNULL)
        {
            y = x;
            if (node->rideNumber < x->rideNumber)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr)
        {
            root = node;
        }
        else if (node->rideNumber < y->rideNumber)
        {
            y->left = node;
        }
        else
        {
            y->right = node;
        }

        if (node->parent == nullptr)
        {
            node->color = 0;
            return nullptr;
        }

        if (node->parent->parent == nullptr)
        {
            return nullptr;
        }

        // FIX THE TREE
        fixInsert(node);
        return node;
    }

    NodePtr getRoot()
    {
        return this->root;
    }

    // DELETE THE NODE FROM THE TREE
    void deleteNode(int data)
    {
        deleteNodeHelper(this->root, data);
    }
};