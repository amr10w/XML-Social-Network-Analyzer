#include <bits/stdc++.h>
#include "Tree.h"

using namespace std;

//------------ Read file (return string) ------------
string readFileToString(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open the file: " << filename << endl;
        return "";  // return empty string on error
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

// 1. Define the Data
struct XmlData {
    std::string tag;
    std::string text;
};

// 2. The Helper Function (The Brains)
// We make this static or part of the class
void node_to_json(Node<string>* node, int indent_level = 0) {
    if (!node) return;

    std::string indent(indent_level * 2, ' ');

    // CASE A: Text Node (Leaf)
    // If the node has no children and has text, print the text
    if (node->children.empty()) {
        std::cout << "\"" << node->data << "\"";
        return;
    }

    // CASE B: Container Node (Object)
    std::cout << "{\n";

    // --- THE "NO MAP" LOGIC STARTS HERE ---
    
    // We need to keep track of tags we have already processed
    // to avoid printing the "post" array multiple times.
    std::vector<std::string> processed_tags;

    for (size_t i = 0; i < node->children.size(); ++i) {
        Node<string>* child = node->children[i];
        std::string current_tag = child->data;

        // Step 1: Check if we already handled this tag
        bool already_processed = false;
        for (const auto& t : processed_tags) {
            if (t == current_tag) {
                already_processed = true;
                break;
            }
        }
        if (already_processed) continue;

        // Step 2: Mark as processed
        processed_tags.push_back(current_tag);

        // Step 3: COUNT occurrences (The "Map" replacement)
        int count = 0;
        for (auto* sibling : node->children) {
            if (sibling->data == current_tag) {
                count++;
            }
        }

        // Print Key
        std::cout << indent << "  \"" << current_tag << "\": ";

        // Step 4: Logic Decision
        if (count > 1) {
            // --- IT IS A LIST (ARRAY) ---
            std::cout << "[\n";
            
            // We must find ALL children with this name and print them
            int printed_count = 0;
            for (auto* sibling : node->children) {
                if (sibling->data == current_tag) {
                    // Recurse
                    std::cout << indent << "    ";
                    node_to_json(sibling, indent_level + 2);
                    
                    printed_count++;
                    if (printed_count < count) std::cout << ","; // Comma between array items
                    std::cout << "\n";
                }
            }
            std::cout << indent << "  ]";
        } 
        else {
            // --- IT IS A SINGLE OBJECT ---
            node_to_json(child, indent_level + 1);
        }

        // Comma handling between keys in the object
        // (This is tricky in C++, simpler to verify if it's the last PROCESSED tag)
        if (i < node->children.size() - 1 && processed_tags.size() < node->children.size()) { 
             // Logic simplified: If we are not at the very end, print comma. 
             // NOTE: Perfect comma handling often requires checking if any *unprocessed* tags remain.
            std::cout << ","; 
        }
        std::cout << "\n";
    }

    std::cout << indent << "}";
}

int main()
{
    string xmlContent = readFileToString("sample.xml");

    vector<Token> tokens = tokenizeXML(xmlContent);

    Tree<string>* tree = buildTree(tokens);

    node_to_json(tree->root);
}