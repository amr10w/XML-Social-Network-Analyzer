#include <string>
#include <vector>

struct PostMatchTopic {
    std::string id;    // post id (may be empty)
    std::string text;  // post text (if available)
};

// Search the XML content for posts where a <topic> equals 'topic' (case-insensitive).
// Returns a vector of PostMatch for matching posts.
std::vector<PostMatchTopic> searchPostsByTopic(const std::string& xmlContent, const std::string& topic);

// Utility to print matches
void printMatches(const std::vector<PostMatchTopic>& matches);
