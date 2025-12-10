#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <string>
#include <stack>


// =====================
// Node Structure
// =====================
template<typename T>
struct Node {
    T data{};
    std::vector<Node<T>*> children ;
    
    Node(T data);
    void addChild(Node<T>* node);
};

// Node IMPLEMENTATION
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
    Tree(const Tree<T>& other);    // deep copy constructor
    ~Tree();

    Node<T>* getRoot() const { return root; }
    
    void print_preorder();
    void print_prettified();
    std::string getPrettifyingString();


private:
    Node<T>* root{};
    Node<T>* copy(Node<T>* item);
    std::string prettifyingString{};
    void destroy(Node<T>* node);
    void print_preorder_helper(Node<T>* node);
    void print_prettified_helper(Node<T>* node, std::string indentation);
};


// ============
// Tree IMPLEMENTATION
// ============

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
void Tree<T>::print_prettified () {
    print_prettified_helper(root,"");
}

template <typename T>
inline std::string Tree<T>::getPrettifyingString()
{
    return prettifyingString;
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


template <typename T>
void Tree<T>::print_prettified_helper(Node<T>* node, std::string indentation) {
    if (!node) return;

    bool preLeaf = (node->children.size() == 1) && (node->children[0]->children.empty());

    if (!preLeaf) {
        prettifyingString+=indentation+"<"+node->data+">\n";
        

        for (int i = 0; i < node->children.size(); i++) {
            print_prettified_helper(node->children[i],indentation + "   ");
        }
        prettifyingString += indentation + "</" + node->data + ">\n";
        
    }
    else {
        prettifyingString+= indentation + "<" +node->data+ ">";
        prettifyingString+= node->children[0]->data;
        prettifyingString += "</"+node->data+">\n";
        
    }
}





// =====================
// Build Tree From Tokens
// =====================


#endif // TREE_H
