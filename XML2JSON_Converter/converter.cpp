#include <bits/stdc++.h>
#include "Tree.h"

using namespace std;

struct XmlElement {
    std::string tag;
    std::string text; // For leaf nodes like <id>1</id>, text is "1"
    
    // Helper to check if it's just text or a container
    bool is_text_node() const { return tag.empty(); }
};

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

//------------ Check if it is a list ------------
// bool is_array(Node<string>* parent, const string& child_tag) {
//     int count = 0;
//     for (auto* child : parent->children) {
//         if (child->data == child_tag) {
//             count++;
//         }
//     }
//     return count > 1;
// }

//------------ Converter ------------
void xml_to_json_dfs(Node<string>* node, int indent_level) {
    string indent(indent_level * 4, ' ');
    
    // 1. Check if this is a purely text node (like <id>1</id>)
    // If it has text content and no children, just print the value.
    if (!node->data.empty() && node->children.empty()) {
        cout << "\"" << node->data<< "\"";
        return;
    }

    cout << "{\n";

    // --- THE "NO MAP" STRATEGY ---
    // We need to group children by name so we handle arrays correctly.
    // We will perform a simpler look-ahead.
    
    // 2. Iterate through children
    for (size_t i = 0; i < node->children.size(); ++i) {
        Node<string>* child = node->children[i];
        string tag = child->data;

        // SKIP LOGIC: If this tag was the same as the previous one, 
        // we already handled it in the loop processing the array. 
        if (i > 0 && node->children[i-1]->data == tag) {
            continue;
        }

        cout << indent << "    \"" << tag << "\": ";

        // 3. CHECK FOR ARRAY (The Look-Ahead)
        if (is_array(node, tag)) {
            // It is an array!
            cout << "[\n";
            
            // Loop through ALL children again to find matches (inefficient but works without Map)
            bool first_item = true;
            for (auto* sibling : node->children) {
                if (sibling->data == tag) {
                    if (!first_item) cout << ",\n";
                    cout << indent << "        ";
                    xml_to_json_dfs(sibling, indent_level + 2); // Recursion
                    first_item = false;
                }
            }
            cout << "\n" << indent << "    ]";
        } 
        else {
            // It is a single object
            xml_to_json_dfs(child, indent_level + 1); // Recursion
        }

        // Comma handling for the parent object keys
        // If there is a next child with a DIFFERENT tag, print comma
        bool is_last_unique_tag = true;
        for(size_t j = i + 1; j < node->children.size(); ++j) {
            if(node->children[j]->data != tag) {
                is_last_unique_tag = false; 
                break;
            }
        }
        
        if (!is_last_unique_tag) {
            cout << ",";
        }
        cout << "\n";
    }

    cout << indent << "}";
}

int main()
{
    string xmlContent = readFileToString("sample.xml");

    vector<Token> tokens = tokenizeXML(xmlContent);

    Tree<string>* tree = buildTree(tokens);
}