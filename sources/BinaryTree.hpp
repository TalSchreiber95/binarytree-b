#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <string>
#include <list>
using namespace std;
namespace ariel
{

    template <class T>
    class BinaryTree
    {

        struct Node
        {
            T info;
            Node *left = nullptr;
            Node *right = nullptr;
            Node(T inf) : info(inf), left(nullptr), right(nullptr) {}
        };
        Node *root = nullptr;

    public:
        BinaryTree<T>() {}

        // Copy the tree by inorder travel recursive
        // Token from: https://stackoverflow.com/questions/10412385/binary-tree-copy-constructor
        void copy(Node *&fRoot, Node *&oRoot)
        {
            if (oRoot == nullptr)
            {
                fRoot = nullptr;
            }
            else
            {
                fRoot = new Node(oRoot->info);
                copy(fRoot->left, oRoot->left);
                copy(fRoot->right, oRoot->right);
            }
        }

        //Copy constractor
        BinaryTree<T>(BinaryTree<T> &o)
        {
            if (o.root != nullptr)
            {
                copy(root, o.root);
            }
            else
            {
                root = nullptr;
            }
        }
        //destructor
        ~BinaryTree<T>()
        {
            if (root != nullptr)
            {
                delete root;
            }
        }
        //move =
        BinaryTree<T> &operator=(BinaryTree<T> &&o) noexcept
        {
            delete root;
            root = o.root;
            o.root = nullptr;
        }
        //move constractor
        BinaryTree<T>(BinaryTree<T> &&o) noexcept
        {
            root = o.root;
            o.root = nullptr;
        }
        //= operator
        BinaryTree<T> &operator=(BinaryTree<T> o) // const & other
        {
            if (this != &o)
            {
                delete root;
                root = new Node(o.root->info);
                copy(root, o.root);
                return *this;
            }
            return *this;
        }

        BinaryTree<T> &add_root(const T &info)
        {
            if (root != nullptr)
            {
                root->info = info;
                return *this;
            }
            root = new Node(info);
            return *this;
        }

        BinaryTree<T> &add_left(const T &parent, const T &child)
        {
            if (root == nullptr)
            {
                throw "Exception: root doesn't exist";
            }
            stack<Node *> stk;
            Node *curr = root;
            while (curr != nullptr || !stk.empty())
            {
                // extreme left
                while (curr != nullptr)
                {
                    stk.push(curr);
                    curr = curr->left;
                }

                curr = stk.top();
                stk.pop();
                if (curr->info == parent)
                {
                    if (curr->left == nullptr)
                    {
                        curr->left = new Node(child);
                    }
                    else
                    {
                        curr->left->info = child;
                    }
                    return *this;
                }
                curr = curr->right;
            }
            throw "Exception: Node doesn't exist";
        }

        BinaryTree<T> &add_right(const T &parent, const T &child)
        {
            if (root == nullptr)
            {
                throw "Exception: root doesn't exist";
            }
            stack<Node *> stk;
            Node *curr = root;
            while (curr != nullptr || !stk.empty())
            {
                while (curr != nullptr)
                {
                    stk.push(curr);
                    curr = curr->left;
                }
                curr = stk.top();
                stk.pop();
                if (curr->info == parent)
                {
                    if (curr->right == nullptr)
                    {
                        curr->right = new Node(child);
                    }
                    else
                    {
                        curr->right->info = child;
                    }
                    return *this;
                }
                curr = curr->right;
            }
            throw "Exception: Node doesn't exist";
        }

        class iterator
        {

        private:
            stack<Node *> stk;
            list<Node *> tmplist;
            string travelType;
            Node *currNode;

        public:
            iterator(string const &tType, Node *ptr = nullptr) : currNode(ptr)
            {
                travelType = tType;
                if (currNode != nullptr)
                {
                    if (travelType == "PreOrder")
                    { //preorder
                        initStackPreOrder();
                    }
                    //inorder
                    else if (travelType == "InOrder")
                    {
                        initStackInOrder();
                    }
                    else
                    {
                        initStackPostOrder();
                    }
                }
            }
            void initStackInOrder()
            {
                while (currNode != nullptr)
                {
                    stk.push(currNode);
                    currNode = currNode->left;
                }
                currNode = stk.top();
                stk.pop();
            }
            void initStackPreOrder()
            {
                if (currNode->right != nullptr)
                {
                    stk.push(currNode->right);
                }
                if (currNode->left != nullptr)
                {
                    stk.push(currNode->left);
                }
            }
            void initStackPostOrder()
            {
                while (currNode != nullptr || !stk.empty())
                {
                    if (currNode == nullptr)
                    {
                        Node *temp = stk.top()->right;
                        if (temp != nullptr)
                        {
                            currNode = temp;
                        }
                        else
                        {
                            temp = stk.top();
                            stk.pop();
                            tmplist.push_back(temp);
                            while (!stk.empty() && temp == stk.top()->right)
                            {
                                temp = stk.top();
                                stk.pop();
                                tmplist.push_back(temp);
                            }
                        }
                    }
                    else
                    {
                        stk.push(currNode);
                        currNode = currNode->left;
                    }
                }
                currNode = tmplist.front();
                tmplist.pop_front();
            }
            T &operator*() const
            {
                return currNode->info;
            }

            T *operator->() const
            {
                return &(currNode->info);
            }

            // ++i;
            iterator &operator++()
            {
                if (travelType == "PreOrder")
                {
                    if (stk.empty())
                    {
                        currNode = nullptr;
                        return *this;
                    }
                    currNode = stk.top();
                    stk.pop();

                    if (currNode->right != nullptr)
                    {
                        stk.push(currNode->right);
                    }
                    if (currNode->left != nullptr)
                    {
                        stk.push(currNode->left);
                    }
                }
                else if (travelType == "InOrder")
                {
                    if (currNode->right == nullptr && stk.empty())
                    {
                        currNode = nullptr;
                    }
                    else
                    {
                        currNode = currNode->right;
                        while (currNode != nullptr)
                        {
                            stk.push(currNode);
                            currNode = currNode->left;
                        }
                        currNode = stk.top();
                        stk.pop();
                    }
                }
                else
                {
                    if (tmplist.size() == 0)
                    {
                        currNode = nullptr;
                    }
                    else
                    {
                        currNode = tmplist.front();
                        tmplist.pop_front();
                    }
                }
                return *this;
            }

            // i++;
            iterator operator++(int)
            {
                iterator it = *this;
                if (travelType == "PreOrder")
                {
                    if (stk.empty())
                    {
                        currNode = nullptr;
                        return it;
                    }
                    currNode = stk.top();
                    stk.pop();
                    if (currNode->right != nullptr)
                    {
                        stk.push(currNode->right);
                    }
                    if (currNode->left != nullptr)
                    {
                        stk.push(currNode->left);
                    }
                }
                else if (travelType == "InOrder")
                {
                    if (currNode->right == nullptr && stk.empty())
                    {
                        currNode = nullptr;
                    }
                    else
                    {
                        currNode = currNode->right;
                        while (currNode != nullptr)
                        {
                            stk.push(currNode);
                            currNode = currNode->left;
                        }
                        currNode = stk.top();
                        stk.pop();
                    }
                }
                else
                {
                    if (tmplist.size() == 0)
                    {
                        currNode = nullptr;
                    }
                    else
                    {
                        currNode = tmplist.front();
                        tmplist.pop_front();
                    }
                }
                return it;
            }

            bool operator==(const iterator &o) const
            {
                return currNode == o.currNode;
            }

            bool operator!=(const iterator &o) const
            {
                return currNode != o.currNode;
            }
        };

        iterator begin_preorder()
        {
            return iterator("PreOrder", root);
        }
        iterator end_preorder()
        {
            return iterator("PreOrder", nullptr);
        }
        iterator begin_inorder()
        {
            return iterator("InOrder", root);
        }
        iterator end_inorder()
        {
            return iterator("InOrder", nullptr);
        }
        iterator begin_postorder()
        {
            return iterator("PostOrder", root);
        }
        iterator end_postorder()
        {
            return iterator("PostOrder", nullptr);
        }
        iterator begin()
        {
            return iterator("InOrder", root);
        }
        iterator end()
        {
            return iterator("InOrder", nullptr);
        }

        // Token from : https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
        void printBT(const string &prefix, const Node *node, bool isLeft) const
        {
            if (node != nullptr)
            {
                cout << prefix;
                if (!isLeft)
                {
                    cout << "├──";
                }
                else
                {
                    cout << "└──";
                }
                cout << node->info << endl;
                // enter the next tree level - left and right branch
                printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
                printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
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
    };
}