#include "SearchWord.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// make string lower
static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return out;
}

// cut space from start and end
static std::string trim(const std::string& s) {
    size_t a = 0;
    while (a < s.size() && std::isspace((unsigned char)s[a])) ++a;
    size_t b = s.size();
    while (b > a && std::isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

// find text inside <tag>...</tag>
static std::string extractTagContent(const std::string& xml, const std::string& tag,
                                     size_t startPos, size_t &outEnd) {
    outEnd = std::string::npos;
    std::string open = "<" + tag;
    size_t openPos = xml.find(open, startPos);
    if (openPos == std::string::npos) return "";
    size_t gt = xml.find('>', openPos);
    if (gt == std::string::npos) return "";
    if (xml[gt-1] == '/') { outEnd = gt + 1; return ""; }
    std::string close = "</" + tag + ">";
    size_t closePos = xml.find(close, gt + 1);
    if (closePos == std::string::npos) return "";
    outEnd = closePos + close.size();
    return xml.substr(gt + 1, closePos - (gt + 1));
}

// get id of post
static std::string extractPostId(const std::string& postBlock) {
    size_t pos = postBlock.find("id=\"");
    if (pos != std::string::npos) {
        pos += 4;
        size_t end = postBlock.find('"', pos);
        if (end != std::string::npos) {
            return trim(postBlock.substr(pos, end - pos));
        }
    }
    size_t endpos;
    std::string inner = extractTagContent(postBlock, "id", 0, endpos);
    if (!inner.empty()) return trim(inner);
    return "";
}

// get text/body of post
static std::string extractPostText(const std::string& postBlock) {
    size_t endpos;
    std::string t = extractTagContent(postBlock, "text", 0, endpos);
    if (!t.empty()) return trim(t);
    t = extractTagContent(postBlock, "body", 0, endpos);
    if (!t.empty()) return trim(t);
    return "";
}

// search posts by word inside text/body
std::vector<PostMatch> searchPostsByWord(const std::string& xmlContent, const std::string& word) {
    std::vector<PostMatch> matches;
    if (word.empty()) return matches;

    std::string wordLower = toLower(word);

    size_t pos = 0;
    while (true) {
        size_t postStart = xmlContent.find("<post", pos);
        if (postStart == std::string::npos) break;
        size_t postClose = xmlContent.find("</post>", postStart);
        if (postClose == std::string::npos) break;
        size_t postEnd = postClose + 7;
        std::string postBlock = xmlContent.substr(postStart, postEnd - postStart);

        std::string text = extractPostText(postBlock);
        if (!text.empty()) {
            std::string textLower = toLower(text);
            if (textLower.find(wordLower) != std::string::npos) {
                PostMatch pm;
                pm.id = extractPostId(postBlock);
                pm.text = text;
                matches.push_back(pm);
            }
        }

        pos = postEnd;
    }

    return matches;
}

void printMatches(const std::vector<PostMatch>& matches) {
    if (matches.empty()) {
        std::cout << "No posts found for the requested word.\n";
        return;
    }
    std::cout << "Found " << matches.size() << " post(s):\n";
    for (const auto &m : matches) {
        if (!m.id.empty())
            std::cout << "PostID: " << m.id << "\n";
        else
            std::cout << "PostID: (unknown)\n";
        if (!m.text.empty())
            std::cout << "Text: " << m.text << "\n";
        std::cout << "----\n";
    }
}

/* -------------------------------
   main just for test
   ------------------------------- */
// int main() {
//     const std::string filename = "sample.xml"; // file name hardcoded
//     const std::string wordToSearch = "this";  // word hardcoded

//     std::ifstream in(filename);
//     if (!in) {
//         std::cerr << "cannot open " << filename << ". put sample.xml near exe.\n";
//         return 1;
//     }
//     std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
//     in.close();

//     std::cout << "Searching file: " << filename << " for word: \"" << wordToSearch << "\"\n\n";
//     std::vector<PostMatch> results = searchPostsByWord(content, wordToSearch);
//     printMatches(results);

//     return 0;
// }
