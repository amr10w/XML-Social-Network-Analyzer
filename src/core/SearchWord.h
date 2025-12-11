#ifndef SEARCHWORD_H
#define SEARCHWORD_H

#include <string>
#include <vector>

// struct to hold match info
struct PostMatch {
    std::string id;   // id of post
    std::string text; // text/body of post
};

// search posts by word inside <text> or <body>
std::vector<PostMatch> searchPostsByWord(const std::string& xmlContent,
                                         const std::string& word);

// print matches to console
void printMatches(const std::vector<PostMatch>& matches);

#endif