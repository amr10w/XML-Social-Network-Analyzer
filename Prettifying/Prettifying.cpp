#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include "Tree.h"

using namespace std;

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

void printToFile(const string& filename, Tree<string>* tree) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file for writing: " << filename << endl;
        return;
    }

    // Save the original cout buffer
    streambuf* oldBuffer = cout.rdbuf();

    // Redirect cout to the file
    cout.rdbuf(file.rdbuf());

    // Use your existing function
    tree->print_prettified();

    // Restore cout
    cout.rdbuf(oldBuffer);

    file.close();
}


int main()
{
    string xmlContent = readFileToString("sample.xml");

    vector<Token> tokens = tokenizeXML(xmlContent);

    Tree<string>* tree = buildTree(tokens);

    printToFile("output.xml",tree);

    return 0;
}
