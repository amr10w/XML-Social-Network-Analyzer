#include "SearchTopic.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

// make string lower
static string toLower(const string& s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(),
              [](unsigned char c){ return tolower(c); });
    return out;
}

// cut space from start and end
static string trim(const string& s) {
    size_t a = 0;
    while (a < s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b = s.size();
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

// find text inside <tag>...</tag>
static string extractTagContent(const string& xml, const string& tag,
                                size_t startPos, size_t &outEnd) {
    outEnd = string::npos;
    string open = "<" + tag;
    size_t openPos = xml.find(open, startPos);
    if (openPos == string::npos) return "";
    size_t gt = xml.find('>', openPos);
    if (gt == string::npos) return "";
    if (xml[gt-1] == '/') { outEnd = gt + 1; return ""; }
    string close = "</" + tag + ">";
    size_t closePos = xml.find(close, gt + 1);
    if (closePos == string::npos) return "";
    outEnd = closePos + close.size();
    return xml.substr(gt + 1, closePos - (gt + 1));
}

// get user id
static string extractUserId(const string& userBlock) {
    size_t endpos;
    string inner = extractTagContent(userBlock, "id", 0, endpos);
    return trim(inner);
}

// get text/body of post
static string extractPostText(const string& postBlock) {
    size_t endpos;
    string t = extractTagContent(postBlock, "text", 0, endpos);
    if (!t.empty()) return trim(t);
    t = extractTagContent(postBlock, "body", 0, endpos);
    if (!t.empty()) return trim(t);
    return "";
}

// helper: check if two strings differ by at most one character
static bool approxEqual(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    int diff = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            diff++;
            if (diff > 1) return false;
        }
    }
    return true;
}

vector<PostMatch> searchPostsByTopic(const string& xmlContent, const string& topic) {
    vector<PostMatch> matches;
    if (topic.empty()) return matches;

    string topicLower = toLower(topic);

    size_t pos = 0;
    while (true) {
        // iterate over <user> blocks
        size_t userStart = xmlContent.find("<user>", pos);
        if (userStart == string::npos) break;
        size_t userClose = xmlContent.find("</user>", userStart);
        if (userClose == string::npos) break;
        size_t userEnd = userClose + 7;
        string userBlock = xmlContent.substr(userStart, userEnd - userStart);

        string userId = extractUserId(userBlock);

        // iterate over <post> blocks inside this user
        size_t postPos = 0;
        while (true) {
            size_t postStart = userBlock.find("<post>", postPos);
            if (postStart == string::npos) break;
            size_t postClose = userBlock.find("</post>", postStart);
            if (postClose == string::npos) break;
            size_t postEnd = postClose + 7;
            string postBlock = userBlock.substr(postStart, postEnd - postStart);

            // search inside <topic> tags
            size_t tpos = 0;
            bool matched = false;
            while (true) {
                size_t topicOpen = postBlock.find("<topic>", tpos);
                if (topicOpen == string::npos) break;
                size_t topicClose = postBlock.find("</topic>", topicOpen);
                if (topicClose == string::npos) break;

                string topicText = trim(postBlock.substr(topicOpen + 7, topicClose - (topicOpen + 7)));
                string topicLowerText = toLower(topicText);

                // word search style: substring match
                if (topicLowerText.find(topicLower) != string::npos) {
                    matched = true;
                    break;
                }

                tpos = topicClose + 8;
            }

            if (matched) {
                PostMatch pm;
                pm.id = userId; // return UserID
                pm.text = extractPostText(postBlock); // optional: include body text
                matches.push_back(pm);
            }

            postPos = postEnd;
        }

        pos = userEnd;
    }

    return matches;
}


void printMatches(const vector<PostMatch>& matches) {
    if (matches.empty()) {
        cout << "No posts found for the requested topic.\n";
        return;
    }
    cout << "Found " << matches.size() << " post(s):\n";
    for (const auto &m : matches) {
        if (!m.id.empty())
            cout << "UserID: " << m.id << "\n";
        else
            cout << "UserID: (unknown)\n";
        if (!m.text.empty())
            cout << "Text: " << m.text << "\n";
        cout << "----\n";
    }
}

/*int main() {
    const string filename = "sample.xml"; // file name hardcoded
    const string topicToSearch = "so"; // topic hardcoded

    ifstream in(filename);
    if (!in) {
        cerr << "cannot open " << filename << ". put sample.xml near exe.\n";
        return 1;
    }
    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cout << "Searching file: " << filename << " for topic: \"" << topicToSearch << "\"\n\n";
    vector<PostMatch> results = searchPostsByTopic(content, topicToSearch);
    printMatches(results);

    return 0;
}*/
