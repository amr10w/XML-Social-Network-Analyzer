#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>

// Helper: Read entire file to string
std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

// Helper: Write string to file
void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot write to file " << filename << "\n";
        return;
    }
    out << content;
    out.close();
}

// Minify XML string
std::string minifyXML(const std::string& xmlContent) {
    std::string result;
    bool insideTag = false;    // Track if we are inside <...>
    bool prevSpace = false;    // Avoid multiple consecutive spaces

    for (size_t i = 0; i < xmlContent.size(); ++i) {
        char c = xmlContent[i];

        if (c == '<') {
            insideTag = true;
            // Remove trailing space before tag
            while (!result.empty() && result.back() == ' ')
                result.pop_back();
            result += c;
        }
        else if (c == '>') {
            insideTag = false;
            result += c;
        }
        else if (insideTag) {
            // Inside tag: preserve everything except newlines/tabs
            if (c != '\n' && c != '\r' && c != '\t')
                result += c;
        }
        else {
            // Outside tag (text content)
            if (!isspace(c)) {
                result += c;
                prevSpace = false;
            }
            else if (!prevSpace) {
                // Collapse multiple spaces into one
                result += ' ';
                prevSpace = true;
            }
        }
    }
    return result;
}

int main() {
    std::string inputFile = "sample.xml";
    std::string outputFile = "minified.xml";

    // 1. Read input file
    std::string xmlContent = readFileToString(inputFile);
    if (xmlContent.empty()) return 1;

    // 2. Minify XML
    std::string minified = minifyXML(xmlContent);

    // 3. Write output
    writeFile(outputFile, minified);

    std::cout << "XML minification completed. Output written to " << outputFile << "\n";
    return 0;
}
