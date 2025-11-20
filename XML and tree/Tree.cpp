#include "Tree.h"
#include <stdexcept>

template <typename T>
Node<T>::Node(T data):data(data)
{
}
template <typename T>
void Node<T>::addChild(Node<T> *node)
{
    children.push_back(node);
}

template<typename T>
Tree<T>::Tree(): root(nullptr) {
}

template <typename T>
Tree<T>::Tree(Node<T> *root):root(root)
{

}

template <typename T>
Tree<T>::Tree(const Tree<T> &other)
{
    root = copy(other.root);
}

template<typename T>
Tree<T>::~Tree() {
    destroy(root);
}

template <typename T>
void Tree<T>::print_preorder()
{
    print_preorder_helper(root);
    std::cout<<"\n";
}

// deep copy
template <typename T>
Node<T> *Tree<T>::copy(Node<T> *item)
{
    if (!item) return nullptr;
    Node<T>* node = new Node<T>(item->data);        
    for (auto e : item->children)
        node->children.push_back(copy(e));
    return node;
}

template <typename T>
void Tree<T>::destroy(Node<T> *node)
{
    if (!node) return;
        for (auto e : node->children) destroy(e);
    delete node;
}

template <typename T>
void Tree<T>::print_preorder_helper(Node<T> *node)
{
    if(!node) return;
    if(node->children.empty())
        std::cout << node->data << " ";
    else
        std::cout << "<" << node->data << "> ";  
    for(int i=0;i<node->children.size();i++)
    {
        print_preorder_helper(node->children[i]);  
    }
}

template struct Node<std::string>;
template class Tree<std::string>;


