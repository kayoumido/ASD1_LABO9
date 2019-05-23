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
    tree.display();

//    cout << "nth element: " << tree.nth_element(0) << endl;
//    cout << "nth element: " << tree.nth_element(1) << endl;
//    cout << "nth element: " << tree.nth_element(2) << endl;
    cout << "nth element: " << tree.nth_element(3) << endl;
    cout << "nth element: " << tree.nth_element(4) << endl;
//    cout << "nth element: " << tree.nth_element(5) << endl;
//    cout << "nth element: " << tree.nth_element(6) << endl;
//    cout << "nth element: " << tree.nth_element(7) << endl;

//    cout << boolalpha << tree.contains(7) << endl;
//
//    cout << "Min : " << tree.min() << endl;
//    tree.deleteMin();
//    tree.display();
//
//
//    tree.deleteElement(10);
//    tree.display();

    return EXIT_SUCCESS;
}