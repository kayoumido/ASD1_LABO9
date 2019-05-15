#include <iostream>
#include "BinarySearchTree.h"

int main() {

    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(7);
    tree.insert(15);
    tree.insert(2);
    tree.insert(8);
    tree.display();

    return EXIT_SUCCESS;
}