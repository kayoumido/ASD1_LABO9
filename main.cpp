#include <iostream>
#include "BinarySearchTree.h"

int main() {

    BinarySearchTree<int> tree;
    tree.insert(11);
    tree.insert(5);
    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(12);
    tree.insert(15);
    tree.insert(13);

    cout << "Test du move constructor - abr2(move(abr))" << endl;
    BinarySearchTree<int> tree2(move(tree));
    cout << "arb2 :" << endl;
    tree2.display();
    cout << "arb1 :" << endl;
    tree.display();

    cout << endl;

    cout << "Test du copy constructor - abr3(abr2)" << endl;
    BinarySearchTree<int> tree3(tree2);
    cout << "arb3 :" << endl;
    tree3.display();

    cout << endl;

    cout << "Test du move operator= - abr4 = move(abr2)" << endl;
    BinarySearchTree<int> tree4 = move(tree2);
    cout << "arb2 :" << endl;
    tree4.display();
    cout << "arb1 :" << endl;
    tree2.display();

    cout << endl;

    cout << "Destruction abr4" << endl;
    tree4.~BinarySearchTree();

    cout << endl;

    cout << "Test du copy operator= - abr2 = abr3" << endl;
    tree2 = tree3;
    cout << "arb2 :" << endl;
    tree2.display();
    cout << "arb1 :" << endl;
    tree3.display();

    cout << endl;

    cout << "Destruction abr3" << endl;
    tree3.~BinarySearchTree();

    cout << endl;

    cout << "Test du swap - abr.swap(abr2)" << endl;
    tree.swap(tree2);
    cout << "arb1 :" << endl;
    tree.display();
    cout << "arb2 :" << endl;
    tree2.display();

    cout << endl;

    cout << "Destruction abr2" << endl;
    tree2.~BinarySearchTree();

    cout << endl;

    cout << "Destruction abr1";
    tree.~BinarySearchTree();


    return EXIT_SUCCESS;
}