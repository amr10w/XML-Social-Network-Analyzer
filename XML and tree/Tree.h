#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <string>

// =====================
// Node Structure
// =====================
template<typename T>
struct Node {
    T data{};
    std::vector<Node<T>*> children;

    Node(T data);
    void addChild(Node<T>* node);
};

template<typename T>
Node<T>::Node(T data) : data(data) {}

template<typename T>
void Node<T>::addChild(Node<T>* node) {
    children.push_back(node);
}


// =====================
// Tree Class
// =====================
template<typename T>
class Tree {
public:
    Tree();
    Tree(Node<T>* root);
    Tree(const Tree<T>& other);
    ~Tree();

    void print_preorder();

    // NEW:
    void print_prettified();
    std::string getPrettifyingString();

private:
    Node<T>* root{};
    Node<T>* copy(Node<T>* item);
    void destroy(Node<T>* node);
    void print_preorder_helper(Node<T>* node);

    // NEW:
    void prettify_helper(Node<T>* node, int depth, std::string& out);
};


// IMPLEMENTATION

template<typename T>
Tree<T>::Tree() : root(nullptr) {}

template<typename T>
Tree<T>::Tree(Node<T>* root) : root(root) {}

template<typename T>
Tree<T>::Tree(const Tree<T>& other) {
    root = copy(other.root);
}

template<typename T>
Tree<T>::~Tree() {
    destroy(root);
}

template<typename T>
void Tree<T>::print_preorder() {
    print_preorder_helper(root);
    std::cout << "\n";
}

template<typename T>
Node<T>* Tree<T>::copy(Node<T>* item) {
    if (!item) return nullptr;

    Node<T>* node = new Node<T>(item->data);
    for (auto e : item->children)
        node->children.push_back(copy(e));

    return node;
}

template<typename T>
void Tree<T>::destroy(Node<T>* node) {
    if (!node) return;

    for (auto e : node->children)
        destroy(e);

    delete node;
}

template<typename T>
void Tree<T>::print_preorder_helper(Node<T>* node) {
    if (!node) return;

    if (node->children.empty())
        std::cout << node->data << " ";
    else
        std::cout << "<" << node->data << "> ";

    for (auto child : node->children)
        print_preorder_helper(child);
}



// =====================
// NEW — Prettifying XML
// =====================

template<typename T>
void Tree<T>::print_prettified() {
    std::string result;
    prettify_helper(root, 0, result);
    std::cout << result << std::endl;
}

template<typename T>
std::string Tree<T>::getPrettifyingString() {
    std::string result;
    prettify_helper(root, 0, result);
    return result;
}

template<typename T>
void Tree<T>::prettify_helper(Node<T>* node, int depth, std::string& out) {
    if (!node) return;

    std::string indent(depth * 2, ' ');

    if (node->children.empty()) {
        out += indent + "<" + node->data + "/>\n";
    } else {
        out += indent + "<" + node->data + ">\n";

        for (auto child : node->children) {
            prettify_helper(child, depth + 1, out);
        }

        out += indent + "</" + node->data + ">\n";
    }
}

#endif // TREE_H
