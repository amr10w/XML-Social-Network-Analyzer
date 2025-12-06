#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

#include <string>
#include <vector>
#include "Tree.h"

class JsonConverter {
public:
    // The function to call from main.cpp
    static std::string convert(Node<std::string>* root);

private:
    // recursive logic
    static void convertToJson(std::string& out, Node<std::string>* root, int indentation);

    // Checks if a node is just a wrapper for text
    static bool isLeafWrapper(Node<std::string>* node);

    template<typename K, typename V>
    struct paair {
        K key;
        V value;
    };
};

#endif