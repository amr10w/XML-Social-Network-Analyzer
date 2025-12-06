#include "JsonConverter.h"
#include <iostream>
#include <algorithm>

using namespace std;
template<typename k, typename v>
struct paair{
  k key;
  v value;
};
string JsonConverter::convert(Node<string>* root) {
  string out = "{\n";
  // Start recursion. Root doesn't need a key in JSON, 
  out += "    \"" + root->data + "\": ";
  convertToJson(out, root, 1);
  out += "\n}\n";
  return out;
}

bool JsonConverter::isLeafWrapper(Node<string>* node) {
    if (node->children.empty()) return true;
    // If any child has children, this is a complex object, not a leaf wrapper
    for (auto child : node->children) {
        if (!child->children.empty()) return false; 
    }
    return true;
}

void JsonConverter::convertToJson(string& out, Node<string>* root, int indentation)
{
  string indent(indentation * 4, ' ');

  // Base case for recursive function (Handle if text node only)
  if(root->children.empty())
  {
    out += "\"" + root->data + "\"";
    return;
  }
  // Case of normal text node
  if (isLeafWrapper(root)) {
      string val = root->children[0]->data; 
      out += "\"" + val + "\"";
      return;
  }
  // Start of object { ... }
  out += "{\n";

  // Sync between 3 vectors instead of Mapping
  // To check if there is multiple of same object to create JSON list
  vector<paair<string, int>> uniqueTags;
  vector<vector<Node<string>*>> grouped;
  for(Node<string>* child : root->children)
  {
    // Safety Check: Can be deleted since there's an error check phase
    //if (child->data.empty()) continue;
    
    bool found = false;
    // Check if it counted already -> exist tag = list
    for(int i = 0; i < uniqueTags.size(); i++)
    {
      if(uniqueTags[i].key == child->data && !child->children.empty())
      {
        uniqueTags[i].value++;
        grouped[i].push_back(child);
        found = true;
        break;
      }
    }
    // Not found -> new tag = new object
    if(!found && !child->children.empty())
    {
      uniqueTags.push_back({child->data, 1});
      grouped.push_back({child});
    }
  }
  // Creating children object
  for(int i = 0; i < uniqueTags.size(); i++)
  {
    string tag = uniqueTags[i].key;
    int cnt = uniqueTags[i].value;
    out += indent + "    \"" + tag + "\": ";
    // Checking if needed a list
    if(cnt > 1)
    {
      out += "[\n";
      for(int j = 0; j < grouped[i].size(); j++)
      {
        out += indent + "        ";
        convertToJson(out, grouped[i][j], indentation + 2);
        if(j < grouped[i].size() - 1) out += ",\n";
      }
      out += "\n" + indent + "    ]";
    }
    // Case just one Object
    else{
      convertToJson(out, grouped[i][0], indentation + 1);
    }
    if(i < uniqueTags.size() - 1) out += ",\n";
  }
  // End of Object
  out += "\n" + indent + "}";
}
