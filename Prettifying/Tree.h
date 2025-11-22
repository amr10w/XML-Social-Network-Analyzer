#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <string>
#include <stack>
using namespace std;

// =====================
// Node Structure
// =====================
template<typename T>
struct Node {
    T data{};
    vector<Node<T>*> children;

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

    void print_preorder();
    void print_prettified();

private:
    Node<T>* root{};
    Node<T>* copy(Node<T>* item);
    void destroy(Node<T>* node);
    void print_preorder_helper(Node<T>* node);
    void print_prettified_helper(Node<T>* node, string indentation);
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
    cout << "\n";
}

template<typename T>
void Tree<T>::print_prettified () {
    print_prettified_helper(root,"");
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
        cout << node->data << " ";
    else
        cout << "<" << node->data << "> ";

    for (auto child : node->children)
        print_preorder_helper(child);
}


template<typename T>
void Tree<T>::print_prettified_helper(Node<T>* node, string indentation) {
    if (!node) return;

    if (!node->children.empty()) {
        cout << indentation << "<" << node->data << ">" << "\n";

        for (auto child : node->children)
            print_prettified_helper(child, indentation + "   ");

        cout << indentation << "</" << node->data << ">" << "\n";
    }
    else {
        cout << indentation << node->data << "\n";
    }
}



// =====================
// XML Token + Parser
// =====================

// Token structure
struct Token {
    string type;   // "tag" or "value"
    string text;
};


// =====================
// XML Tokenizer
// =====================
inline vector<Token> tokenizeXML(const string& xmlContent)
{
    vector<Token> tokens;

    for (int i = 0; i < xmlContent.length(); i++)
    {
        if (xmlContent[i] == '<')
        {
            Token token;
            token.type = "tag";
            i++;
            while (xmlContent[i] != '>')
            {
                token.text += xmlContent[i];
                i++;
            }
            tokens.push_back(token);
        }
        else if (xmlContent[i] == '\n' || xmlContent[i] == ' ')
        {
            continue;
        }
        else
        {
            Token token;
            token.type = "value";
            while (xmlContent[i] != '<')
            {
                token.text += xmlContent[i];
                i++;
            }
            i--; // step back so the loop sees '<'
            tokens.push_back(token);
        }
    }

    return tokens;
}



// =====================
// Build Tree From Tokens
// =====================
inline Tree<string>* buildTree(const vector<Token>& tokens)
{
    Node<string>* root = nullptr;
    stack<Node<string>*> st;

    for (auto& e : tokens)
    {
        if (e.type == "tag")
        {
            // Opening tag: <tag>
            if (!e.text.empty() && e.text[0] != '/')
            {
                Node<string>* node = new Node<string>(e.text);

                if (!st.empty())
                {
                    Node<string>* parent = st.top();
                    parent->addChild(node);
                }
                else
                {
                    root = node;
                }

                st.push(node);
            }
            // Closing tag: </tag>
            else if (!e.text.empty())
            {
                st.pop();
            }
        }
        else // value
        {
            Node<string>* node = new Node<string>(e.text);
            Node<string>* parent = st.top();
            parent->addChild(node);
        }
    }

    return new Tree<string>(root);
}

#endif // TREE_H
