#pragma once
#include <iostream>
#include <stack>
#include <queue>

using namespace std;

namespace ariel
{
    template <typename T>
    class BinaryTree
    {
    private:
        struct Node
        {
            T data;
            Node *left;
            Node *right;
            Node(const T &child) : data(child), left(nullptr), right(nullptr) {}
            // Node's copy constructor
            // Node(const Node<T> &n ) : data(n.data), left(n.left), right(n.right) {}
            //Node's Distructor
            ~Node()
            {
                delete left;
                delete right;
            }
        };

        Node *root;

    public:
        /***************CLASS ITERATOR***************/

        class iteratorT
        {

        private:
            Node *currNode;
            queue<Node *> it;

            void inOrderInitQ()
            {
                if (currNode == nullptr)
                {
                    return;
                }

                stack<Node *> stk;
                Node *curr = currNode;

                while (curr != nullptr || !stk.empty())
                {
                    while (curr != nullptr)
                    {
                        stk.push(curr);
                        curr = curr->left;
                    }

                    curr = stk.top();
                    stk.pop();
                    it.push(curr);
                    curr = curr->right;
                }

                it.push(nullptr);
                currNode = it.front();
                it.pop();
            }

            void preOrderInitQ()
            {
                if (currNode == nullptr)
                {
                    return;
                }
                stack<Node *> stk;
                stk.push(currNode);

                while (!stk.empty())
                {
                    Node *node = stk.top();
                    it.push(node);
                    stk.pop();

                    if (node->right)
                    {
                        stk.push(node->right);
                    }
                    if (node->left)
                    {
                        stk.push(node->left);
                    }
                }

                it.push(nullptr);
                currNode = it.front();
                it.pop();
            }
            void postOrderInitQ()
            {
                if (currNode == nullptr)
                {
                    return;
                }

                stack<Node *> s1;
                stack<Node *> s2;

                s1.push(currNode);
                Node *node = nullptr;

                while (!s1.empty())
                {
                    node = s1.top();
                    s1.pop();
                    s2.push(node);

                    if (node->left)
                    {
                        s1.push(node->left);
                    }
                    if (node->right)
                    {
                        s1.push(node->right);
                    }
                }

                while (!s2.empty())
                {

                    node = s2.top();
                    it.push(node);
                    s2.pop();
                }

                it.push(nullptr);
                currNode = it.front();
                it.pop();
            }

        public:
            iteratorT(Node *ptr, string const &order) : currNode(ptr)
            {
                if (order == "PostOrder")
                {
                    postOrderInitQ();
                }
                else if (order == "InOrder")
                {
                    inOrderInitQ();
                }
                else if (order == "PreOrder")
                {
                    preOrderInitQ();
                }
                else
                {
                    throw "Exception: invalid order traveler type!";
                }
            }

            T &operator*() const { return currNode->data; }

            const T *operator->() const { return &(currNode->data); }

            // ++i;
            iteratorT &operator++()
            {
                if (!it.empty())
                {
                    currNode = it.front();
                    it.pop();
                }
                return *this;
            }

            // i++;
            iteratorT operator++(int)
            {
                iteratorT tmp = *this;
                currNode = it.front();
                it.pop();
                return tmp;
            }

            bool operator==(const iteratorT &other) const { return currNode == other.currNode; }

            bool operator!=(const iteratorT &other) const { return currNode != other.currNode; }

            Node *get_node() { return currNode; }
        };

        /*END OF CLASS ITERATOR*/
        // BinaryTree& operator*() const { return this; }

        BinaryTree() : root(nullptr) {}
        // Copy constructor
        BinaryTree(BinaryTree<T> &bt) /*: root(bt.root)*/
        {
            root=new Node(bt.root->data);

            // queue<Node *> it;
            // cout<<"\nthe root's data: "<< root->data<<endl;
            // if (root == nullptr)
            // {
            //     return;
            // }

            // stack<Node *> stk;
            Node *curr = bt.root;
            // Node *parent = nullptr;
            // while (curr != nullptr || !stk.empty())
            // {
                while (curr!= nullptr)
                {
                    add_left(bt.root->data,bt.root->left->data);
                    // curr=bt.root->left;
            //         stk.push(curr);
            //         parent = new Node(curr->data);
                    curr = curr->left;
                    // add_left_copyConstructor(parent->data,curr->data);
                }
                

            //     curr = stk.top();
            //     stk.pop();
            //     it.push(curr);
            //     curr = curr->right;
            //     if (curr != nullptr)
            //     {
            //         // add_right_copyConstructor(parent->data,curr->data);
            //     }
            // }

            // it.push(nullptr);
            // root = it.front();
            // it.pop();
        }

        ~BinaryTree()
        {
            delete root;
        }
        // BinaryTree &operator=(BinaryTree other){
        //     root=other.root;
        // }
 
        // Token from: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
        void printBT(const string &prefix, const Node *node, bool isLeft) const
        {
            if (node != nullptr)
            {
                cout << prefix;
                if (isLeft)
                {
                    cout << "├──";
                }
                else
                {
                    cout << "└──";
                }
                cout << node->data << endl;
                // enter the next tree level - left and right branch
                printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
                printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
            }
        }

        void printBT(const Node *node) const { printBT("", node, false); }

        friend ostream &operator<<(ostream &os, const BinaryTree &b)
        {
            os << "\nTal's binary Tree:\n"
               << endl;
            // pass the root node of your binary tree
            b.printBT(b.root);
            return os;
        }

        iteratorT begin_preorder() const { return iteratorT(root, "PreOrder"); }
        iteratorT end_preorder() const { return iteratorT(nullptr, "PreOrder"); }
        iteratorT begin_inorder() const { return iteratorT(root, "InOrder"); }
        iteratorT end_inorder() const { return iteratorT(nullptr, "InOrder"); }
        iteratorT begin_postorder() const { return iteratorT(root, "PostOrder"); }
        iteratorT end_postorder() const { return iteratorT(nullptr, "PostOrder"); }
        iteratorT begin() const { return begin_inorder(); }
        iteratorT end() const { return end_inorder(); }

        BinaryTree &add_root(T order)
        {
            if (root)
            {
                root->data = order;
            }
            else
            {
                root = new Node{order};
            }
            return *this;
        }

        BinaryTree &add_left(T parent, T child)
        {
            if (root == nullptr)
            {
                throw invalid_argument("Exception: No root found on this tree");
            }
            Node *curr = nullptr;
            for (auto i = begin_inorder(); i != end_inorder(); ++i)
            {
                if (*i == parent)
                {
                    curr = i.get_node();
                    break;
                }
            }
            if (curr == nullptr)
            {
                throw invalid_argument("Exception: Node not found in the tree");
            }

            if (curr->left)
            {
                curr->left->data = child;
            }
            else
            {
                curr->left = new Node{child};
            }
            return *this;
        }
        BinaryTree &add_left_copyConstructor(T parent, T child)
        {
            //  if (root == nullptr)
            // {
            //     throw invalid_argument("Exception: No root found on this tree");
            // }
            // Node *curr = nullptr;
            // for (auto i = begin_inorder(); i != end_inorder(); ++i)
            // {
            //     if (*i == parent)
            //     {
            //         curr = i.get_node();
            //         break;
            //     }
            // }
            // if (curr == nullptr)
            // {
            //     throw invalid_argument("Exception: Node not found in the tree");
            // }

            // curr->left = new Node{child};
            return *this;
        }

        BinaryTree &add_right(T parent, T child)
        {
            if (root == nullptr)
            {
                throw invalid_argument("Exception: No root found on this tree");
            }
            Node *curr = nullptr;
            for (auto i = begin_inorder(); i != end_inorder(); ++i)
            {
                if (*i == parent)
                {
                    curr = i.get_node();
                    break;
                }
            }
            if (curr == nullptr)
            {
                throw invalid_argument("Exception: Node not found in the tree");
            }
            if (curr->right)
            {
                curr->right->data = child;
            }
            else
            {
                curr->right = new Node{child};
            }
            return *this;
        }
        BinaryTree &add_right_copyConstructor(T parent, T child)
        {
            if (root == nullptr)
            {
                throw invalid_argument("Exception: No root found on this tree");
            }
            Node *curr = nullptr;
            for (auto i = begin_inorder(); i != end_inorder(); ++i)
            {
                if (*i == parent)
                {
                    curr = i.get_node();
                    break;
                }
            }
            if (curr == nullptr)
            {
                throw invalid_argument("Exception: Node not found in the tree");
            }

            curr->right = new Node{child};
            return *this;
        }
    };
}