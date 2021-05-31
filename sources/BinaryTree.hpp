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
            T data;
            Node *left = nullptr;
            Node *right = nullptr;
            Node(T child) : data(child), left(nullptr), right(nullptr) {}
        };
        Node *root = nullptr;

    public:
        BinaryTree<T>() {}

        // Copy the tree by inorder travel recursive
        // Token from: https://stackoverflow.com/questions/10412385/binary-tree-copy-constructor
        void copy(Node *&fRoot, Node *&oRoot)
        {
            if (oRoot == NULL)
            {
                fRoot = NULL;
            }
            else
            {
                fRoot = new Node(oRoot->data);
                copy(fRoot->left, oRoot->left);
                copy(fRoot->right, oRoot->right);
            }
        }

        //Copy constractor
        BinaryTree<T>(BinaryTree<T> &other)
        {
            if (other.root == NULL)
            {
                root = NULL;
            }
            else
            {
                copy(root, other.root);
            }
        }
        //distractor
        ~BinaryTree<T>()
        {
            if (root != nullptr)
            {
                delete root;
            }
        }
        //move =
        BinaryTree<T> &operator=(BinaryTree<T> &&other) noexcept
        {
            delete root;
            root = other.root;
            other.root = nullptr;
        }
        //move constractor
        BinaryTree<T>(BinaryTree<T> &&other) noexcept
        {
            root = other.root;
            other.root = nullptr;
        }
        //= operator
        BinaryTree<T> &operator=(BinaryTree<T> other)
        {
            if (this == &other)
            {
                return *this;
            }
            delete root;
            root = new Node(other.root->data);
            copy(this->root, other.root);
            return *this;
        }

        BinaryTree<T> &add_root(const T &data)
        {
            if (root == nullptr)
            {
                root = new Node(data);
            }
            else
            {
                root->data = data;
            }
            return *this;
        }

        BinaryTree<T> &add_left(const T &parent, const T &child)
        {
            if (root == nullptr)
            {
                throw(string) "Exception: root doesn't exist";
            }
            stack<Node *> stk;
            Node *curr = root;
            while (curr != nullptr || !stk.empty())
            {

                while (curr != NULL)
                {
                    stk.push(curr);
                    curr = curr->left;
                }

                curr = stk.top();
                stk.pop();
                if (curr->data == parent)
                {
                    if (curr->left == nullptr)
                    {
                        curr->left = new Node(child);
                    }
                    else
                    {
                        curr->left->data = child;
                    }
                    return *this;
                }
                curr = curr->right;
            }
            throw invalid_argument{"Exception: Node doesn't exist"};
        }

        BinaryTree<T> &add_right(const T &parent, const T &child)
        {
            if (root == nullptr)
            {
                throw(string) "Exception: root doesn't exist";
            }
            stack<Node *> stk;
            Node *curr = root;
            while (curr != nullptr || !stk.empty())
            {
                while (curr != NULL)
                {
                    stk.push(curr);
                    curr = curr->left;
                }
                curr = stk.top();
                stk.pop();
                if (curr->data == parent)
                {
                    if (curr->right == nullptr)
                    {
                        curr->right = new Node(child);
                    }
                    else
                    {
                        curr->right->data = child;
                    }
                    return *this;
                }
                curr = curr->right;
            }
            throw invalid_argument{"Exception: Node doesn't exist"};
        }

        class iterator
        {

        private:
            stack<Node *> stk;
            string Order;
            Node *currNode;
            list<Node *> PostList;

        public:
            iterator(string const &order, Node *ptr = nullptr) : currNode(ptr)
            {
                Order = order;
                if (currNode != nullptr)
                {
                    if (order == "PreOrder")
                    { //preorder
                        initStackPreOrder();
                    }
                    //inorder
                    else if (order == "InOrder")
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
                    if (currNode != nullptr)
                    {
                        stk.push(currNode);
                        currNode = currNode->left;
                    }
                    else
                    {
                        Node *temp = stk.top()->right;
                        if (temp == nullptr)
                        {
                            temp = stk.top();
                            stk.pop();
                            PostList.push_back(temp);
                            while (!stk.empty() && temp == stk.top()->right)
                            {
                                temp = stk.top();
                                stk.pop();
                                PostList.push_back(temp);
                            }
                        }
                        else
                        {
                            currNode = temp;
                        }
                    }
                }
                currNode = PostList.front();
                PostList.pop_front();
            }
            T &operator*() const
            {
                return currNode->data;
            }

            T *operator->() const
            {
                return &(currNode->data);
            }

            // ++i;
            iterator &operator++()
            {
                if (Order == "PreOrder")
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
                else if (Order == "InOrder")
                {
                    if (!stk.empty() || currNode->right != nullptr)
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
                    else
                    {
                        currNode = nullptr;
                    }
                }
                else
                {
                    if (PostList.size() != 0)
                    {
                        currNode = PostList.front();
                        PostList.pop_front();
                    }
                    else
                    {
                        currNode = nullptr;
                    }
                }
                return *this;
            }

            // i++;
            iterator operator++(int)
            {
                iterator tmpIT = *this;
                if (Order == "PreOrder")
                {
                    if (stk.empty())
                    {
                        currNode = nullptr;
                        return tmpIT;
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
                else if (Order == "InOrder")
                {
                    if (!stk.empty() || currNode->right != nullptr)
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
                    else
                    {
                        currNode = nullptr;
                    }
                }
                else
                {
                    if (PostList.size() != 0)
                    {
                        currNode = PostList.front();
                        PostList.pop_front();
                    }
                    else
                    {
                        currNode = nullptr;
                    }
                }
                return tmpIT;
            }

            bool operator==(const iterator &other) const
            {
                return currNode == other.currNode;
            }

            bool operator!=(const iterator &other) const
            {
                return currNode != other.currNode;
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
                cout << node->data << endl;
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