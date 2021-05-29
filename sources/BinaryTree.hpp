
#include <iostream>
#include <stack>
#include <queue>

using namespace std;

namespace ariel
{
    const int INORDER = 0;
    const int PREORDER = 1;
    const int POSTORDER = 2;

    template <typename T>
    class BinaryTree
    {
    private:
        struct Node
        {
            T data;
            Node *left;
            Node *right;
            Node(const T &v) : data(v), left(nullptr), right(nullptr) {}

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

                stack<Node *> s;
                Node *curr = currNode;

                while (curr != nullptr || s.empty() == false)
                {
                    while (curr != nullptr)
                    {
                        s.push(curr);
                        curr = curr->left;
                    }

                    curr = s.top();
                    it.push(curr);
                    s.pop();

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
                stack<Node *> s;
                s.push(currNode);

                while (!s.empty())
                {
                    Node *node = s.top();
                    it.push(node);
                    s.pop();

                    if (node->right)
                    {
                        s.push(node->right);
                    }
                    if (node->left)
                    {
                        s.push(node->left);
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
            iteratorT(Node *ptr, string const& order) : currNode(ptr)
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
                else{
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

        BinaryTree() : root(nullptr) {}
        ~BinaryTree()
        {
            delete root;
        }
        // Copy constructor
        // BinaryTree(BinaryTree<T>& bt){ 
        //     root=new Node();
        //     root=bt.root;
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
            os << "\nTal's binary Tree:\n" << endl;
            // pass the root node of your binary tree
            b.printBT(b.root);
            return os;
        }

        iteratorT begin_preorder() const { return iteratorT(root, "PreOrder"); }
        iteratorT end_preorder() const { return iteratorT(nullptr,"PreOrder"); }
        iteratorT begin_inorder() const { return iteratorT(root, "InOrder"); }
        iteratorT end_inorder() const { return iteratorT(nullptr,"InOrder"); }
        iteratorT begin_postorder() const { return iteratorT(root, "PostOrder"); }
        iteratorT end_postorder() const { return iteratorT(nullptr,"PostOrder"); }
        iteratorT begin() const { return begin_inorder(); }
        iteratorT end() const { return end_inorder(); }

        BinaryTree &add_root(T order)
        {
            if(root){
                root->data = order;
            }
            else{
                root = new Node{order};
            }
            return *this;
        }

        BinaryTree &add_left(T p, T v)
        {
            if (root == nullptr)
            {
                throw invalid_argument("Exception: No root found on this tree");
            }
            Node *curr = nullptr;
            for (auto i = begin_inorder(); i != end_inorder(); ++i)
            {
                if (*i == p)
                {
                    curr = i.get_node();
                    break;
                }
            }
            if (curr == nullptr)
            {
                throw invalid_argument("Exception: Node not found in the tree");
            }

            if(curr->left){ 
                curr->left->data = v;
            }
            else{
                curr->left = new Node{v};
            }
            return *this;
        }

        BinaryTree &add_right(T p, T v)
        {
            if (root == nullptr)
            {
                throw invalid_argument("Exception: No root found on this tree");
            }
            Node *curr = nullptr;
            for (auto i = begin_inorder(); i != end_inorder(); ++i)
            {
                if (*i == p)
                {
                    curr = i.get_node();
                    break;
                }
            }
            if (curr == nullptr)
            {
                throw invalid_argument("Exception: Node not found in the tree");
            }
            if(curr->right){
                curr->right->data = v;
            }
            else{
                curr->right = new Node{v};
            }
            return *this;
        }
    };
}