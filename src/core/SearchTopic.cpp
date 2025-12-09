#include "SearchTopic.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// make string lower
static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });
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

// find text inside <tag>...</tag> from pos
// give empty if not found. also return end pos after close tag
static std::string extractTagContent(const std::string& xml, const std::string& tag, size_t startPos, size_t &outEnd) {
    outEnd = std::string::npos;
    std::string open = "<" + tag;
    size_t openPos = xml.find(open, startPos);
    if (openPos == std::string::npos) return "";
    // find '>' end of open tag
    size_t gt = xml.find('>', openPos);
    if (gt == std::string::npos) return "";
    // check self close tag
    if (xml[gt-1] == '/') { outEnd = gt + 1; return ""; }
    std::string close = "</" + tag + ">";
    size_t closePos = xml.find(close, gt + 1);
    if (closePos == std::string::npos) return "";
    outEnd = closePos + close.size();
    return xml.substr(gt + 1, closePos - (gt + 1));
}

// try get id of <post>. can be <post id="X"> or inside <id>X</id>
static std::string extractPostId(const std::string& postBlock) {
    // try attr id="..."
    size_t pos = postBlock.find("id=\"");
    if (pos != std::string::npos) {
        pos += 4;
        size_t end = postBlock.find('"', pos);
        if (end != std::string::npos) {
            return trim(postBlock.substr(pos, end - pos));
        }
    }
    // try <id>...</id>
    size_t endpos;
    std::string inner = extractTagContent(postBlock, "id", 0, endpos);
    if (!inner.empty()) return trim(inner);
    return ""; // not found
}

// get first <text>...</text> in postBlock
static std::string extractPostText(const std::string& postBlock) {
    size_t endpos;
    std::string t = extractTagContent(postBlock, "text", 0, endpos);
    if (!t.empty()) return trim(t);
    // maybe <body>...</body>
    t = extractTagContent(postBlock, "body", 0, endpos);
    if (!t.empty()) return trim(t);
    return "";
}

std::vector<PostMatchTopic> searchPostsByTopic(const std::string& xmlContent, const std::string& topic) {
    std::vector<PostMatchTopic> matches;
    if (topic.empty()) return matches;

    std::string topicLower = toLower(topic);

    size_t pos = 0;
    while (true) {
        // find next <post
        size_t postStart = xmlContent.find("<post", pos);
        if (postStart == std::string::npos) break;
        // find </post>
        size_t postClose = xmlContent.find("</post>", postStart);
        if (postClose == std::string::npos) break;
        size_t postEnd = postClose + 7; // len of "</post>"
        std::string postBlock = xmlContent.substr(postStart, postEnd - postStart);

        // find all <topic>...</topic> inside postBlock
        size_t tpos = 0;
        bool matched = false;
        while (true) {
            size_t topicOpen = postBlock.find("<topic", tpos);
            if (topicOpen == std::string::npos) break;

            // make sure is "topic" not "topics"
            size_t boundary = topicOpen + 6;
            if (boundary >= postBlock.size()) break;
            char next = postBlock[boundary];
            if (!(next == '>' || next == '/' || std::isspace(static_cast<unsigned char>(next)))) {
                // false match like "<topics"
                tpos = topicOpen + 1;
                continue;
            }

            // find '>' end of open tag
            size_t gt = postBlock.find('>', topicOpen);
            if (gt == std::string::npos) break;

            // check self close <topic ... />
            if (gt > topicOpen && postBlock[gt - 1] == '/') {
                tpos = gt + 1;
                continue;
            }

            // find </topic>
            size_t topicClose = postBlock.find("</topic>", gt + 1);
            if (topicClose == std::string::npos) break;

            std::string topicText = trim(postBlock.substr(gt + 1, topicClose - (gt + 1)));
            if (toLower(topicText) == topicLower) {
                matched = true;
                break;
            }

            tpos = topicClose + 8; // move after "</topic>"
        }

        if (matched) {
            PostMatchTopic pm;
            pm.id = extractPostId(postBlock);
            pm.text = extractPostText(postBlock);
            matches.push_back(pm);
        }

        pos = postEnd;
    }

    return matches;
}

void printMatches(const std::vector<PostMatchTopic>& matches) {
    if (matches.empty()) {
        std::cout << "No posts found for the requested topic.\n";
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
/*int main() {
    const std::string filename = "sample.xml"; // file name hardcoded
    const std::string topicToSearch = "study"; // topic hardcoded

    // read file
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "cannot open " << filename << ". put sample.xml near exe.\n";
        return 1;
    }
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    std::cout << "Searching file: " << filename << " for topic: \"" << topicToSearch << "\"\n\n";
    std::vector<PostMatch> results = searchPostsByTopic(content, topicToSearch);
    printMatches(results);

    return 0;
}*/
