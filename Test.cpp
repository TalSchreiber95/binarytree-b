#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "doctest.h"
using namespace std;

#include "BinaryTree.hpp"
using namespace ariel;

TEST_CASE("int Binary tree")
{
    BinaryTree<int> btInt;
    CHECK_NOTHROW(btInt.add_root(0));
    CHECK_NOTHROW(btInt.add_root(1));
    CHECK_NOTHROW(btInt.add_left(1, 3));
    CHECK_NOTHROW(btInt.add_left(1, 2));
    CHECK_NOTHROW(btInt.add_right(1, 4));
    CHECK_THROWS(btInt.add_left(7, 4)); //7 doesn't exist
    CHECK_NOTHROW(btInt.add_right(1, 3));
    CHECK_NOTHROW(btInt.add_right(3, 7));
    CHECK_NOTHROW(btInt.add_left(3, 6));
    CHECK_NOTHROW(btInt.add_right(2, 5));
    CHECK_NOTHROW(btInt.add_left(2, 4));

    int inArr[] = {4, 2, 5, 1, 6, 3, 7};
    int preArr[] = {1, 2, 4, 5, 3, 6, 7};
    int postArr[] = {4, 5, 2, 6, 7, 3, 1};
    int i = 0;
    SUBCASE("check operator == , and check itertor increment")
    {
        for (auto it = btInt.begin_preorder(); it != btInt.end_preorder(); ++it)
        {
            CHECK((*it) == preArr[i]);
            i++;
        }
        i = 0;
        for (const int &element : btInt)
        {
            CHECK(element == inArr[i]);
            i++;
        }
        i = 0;
        for (auto it = btInt.begin_postorder(); it != btInt.end_postorder(); ++it)
        {
            CHECK((*it) == postArr[i]);
            i++;
        }
    }

    SUBCASE("add to a twice node exist")
    {
        CHECK_NOTHROW(btInt.add_right(5, 4));
        CHECK_NOTHROW(btInt.add_left(4, 1)); // there is a twice 4 nodes

        string in1 = "1,4,2,5,4,1,6,3,7,", in2 = "4,2,5,1,4,1,6,3,7,";
        string pre1 = "1,2,4,1,5,4,3,6,7,", pre2 = "1,2,4,5,4,1,3,6,7,";
        string post1 = "1,4,4,5,2,6,7,3,1,", post2 = "4,1,4,5,2,6,7,3,1,";

        string ansPre, ansIn, ansPost;

        for (auto it = btInt.begin_postorder(); it != btInt.end_postorder(); ++it)
        {
            ansPost += to_string(*it);
            ansPost += ",";
        }
        for (auto it = btInt.begin_inorder(); it != btInt.end_inorder(); ++it)
        {
            ansIn += to_string(*it);
            ansIn += ",";
        }
        for (auto it = btInt.begin_preorder(); it != btInt.end_preorder(); it++)
        {
            ansPre += to_string(*it);
            ansPre += ",";
        }

        CHECK(((ansPre == pre1) || (ansPre == pre2)));
        CHECK(((ansIn == in1) || (ansIn == in2)));
        CHECK(((ansPost == post1) || (ansPost == post2)));
    }
    cout << "check first \n";

    CHECK_NOTHROW(cout << btInt << endl);
}

TEST_CASE("char Binary tree")
{
    BinaryTree<char> btChar;
    CHECK_THROWS(btChar.add_right('t','C'));//there is no nodes in the tree
    CHECK_NOTHROW(btChar.add_root('t'));
    CHECK_NOTHROW(btChar.add_right('t','C'));
    CHECK_NOTHROW(btChar.add_left('t','v'));
    CHECK_NOTHROW(btChar.add_left('v','D'));
    CHECK_NOTHROW(btChar.add_right('v','u'));
    CHECK_THROWS(btChar.add_right('c','u')); // node doesn't exist
    CHECK_THROWS(btChar.add_right('*','o')); // node doesn't exist
    CHECK_THROWS(btChar.add_left('=','+')); // node doesn't exist

    SUBCASE("check the tree is correct")
    {
        char arrPre[]={'t','v','D','u','C'};
        char arrIn[]={'D','v','u','t','C'};
        char arrPost[]={'D','u','v','C','t'};
        int i=0;
        for (auto it=btChar.begin_postorder(); it!=btChar.end_postorder(); it++) {
            CHECK((*it)==arrPost[i]);
            i++;
        }
        i=0;
        for (auto it=btChar.begin_inorder(); it!=btChar.end_inorder(); it++) {
            CHECK((*it)==arrIn[i]);
            i++;
        }
        i=0;
        for (auto it=btChar.begin_preorder(); it!=btChar.end_preorder(); it++) {
            CHECK((*it)==arrPre[i]);
            i++;
        }
    }

    SUBCASE("check override by add_right/add_left")
    {
          CHECK_NOTHROW(btChar.add_root('a'));
        CHECK_NOTHROW(btChar.add_left('a','b'));

        CHECK_THROWS(btChar.add_left('A','0'));
        CHECK_THROWS(btChar.add_left('B','6'));

        char arrPre[]={'a','b','D','u','C'};
        char arrIn[]={'D','b','u','a','C'};
        char arrPost[]={'D','u','b','C','a'};
        int i=0;
            for (auto it=btChar.begin_postorder(); it!=btChar.end_postorder(); ++it) {
                CHECK((*it)==arrPost[i]);
                i++;
            }
            i=0;
            for (const char& element: btChar) { 
                CHECK(element==arrIn[i]);
                i++;
            }
            i=0;
            for (auto it=btChar.begin_preorder(); it!=btChar.end_preorder(); ++it) {
                CHECK((*it)==arrPre[i]);
                i++;
            }
    }
    SUBCASE("check copy constructor")
    {
        cout<< "\nstarting to check copy constructor\n";
        BinaryTree<char> *btCharCopy = new BinaryTree(btChar);
        cout<< "\nthe original tree \n";
        CHECK_NOTHROW(cout << btChar << endl);
        cout<< "\nthe copy constructor tree \n";
        CHECK_NOTHROW(cout << *btCharCopy << endl);
        // cout<<"\n adding to the original tree\n"<<endl;
        // CHECK_NOTHROW(btChar.add_left('t','m'));        
        // cout<< "\nthe original tree \n";
        // CHECK_NOTHROW(cout << btChar << endl);
        // cout<< "\nthe copy constructor tree \n";
        // CHECK_NOTHROW(cout << *btCharCopy << endl);
        cout<< "\nending to check copy constructor\n";


    }

    // CHECK_NOTHROW(cout << btChar << endl);
}