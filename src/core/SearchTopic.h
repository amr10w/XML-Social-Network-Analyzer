#ifndef SEARCHTOPIC_H
#define SEARCHTOPIC_H

#include <string>
#include <vector>
#include "SearchWord.h"



// Search the XML content for posts where a <topic> equals 'topic' (case-insensitive).
// Returns a vector of PostMatch for matching posts.
std::vector<PostMatch> searchPostsByTopic(const std::string& xmlContent, const std::string& topic);

// Utility to print matches
void printMatchesTopic(const std::vector<PostMatch>& matches);

#endif