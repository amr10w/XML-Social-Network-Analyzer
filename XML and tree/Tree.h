#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <string>

// --- Node Structure (Struct) ---
template<typename T>
struct Node {
    T data {};
    std::vector<Node<T>*> children;
    
    Node(T data);
    void addChild(Node<T>* node);

};

// --- Tree Class (Class) ---
template<typename T>
class Tree {
public:
    Tree();
    Tree(Node<T>* root);
    Tree(const Tree<T>& other); // deep copy (copy constructor)
    ~Tree();
    void print_preorder();

    
private:
    Node<T>* root {};
    Node<T>* copy(Node<T>* item);
    void destroy(Node<T>* node);
    void print_preorder_helper(Node<T> *node);

};
extern template class Tree<std::string>;

#endif // TREE_H


