#include <iostream>
#include <algorithm>
#include <set>
#include "Mutual.h"

// ----------------------------------------------------
// Compute mutual followers of given IDs
// ----------------------------------------------------
std::vector<int> computeMutualFollowers(Graph& graph, const std::vector<int>& ids)
{
    if (ids.empty()) return {};

    // Get followers of the first user
    std::vector<int> result = graph.getNeighbors(ids[0]);
    std::sort(result.begin(), result.end());

    // Intersect with remaining users
    for (int i = 1; i < ids.size(); i++)
    {
        std::vector<int> current = graph.getNeighbors(ids[i]);
        std::sort(current.begin(), current.end());

        std::vector<int> temp;
        std::set_intersection(result.begin(), result.end(),
                              current.begin(), current.end(),
                              std::back_inserter(temp));

        result = temp;
    }

    return result;
}

// ----------------------------------------------------
// Print vector helper
// ----------------------------------------------------
void printVector(const std::vector<int>& v)
{
    if (v.empty())
    {
        std::cout << "No mutual followers found.\n";
        return;
    }

    std::cout << "Mutual followers: ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << "\n";
}

// // ----------------------------------------------------
// // MAIN for testing
// // ----------------------------------------------------
// int main()
// {
//     // Hardcoded input file + IDs for testing
//     std::string file = "test.xml";
//     std::vector<int> ids = {8,3,9};

//     std::cout << "Loading graph from: " << file << "\n";
//     Graph g(file);

//     std::cout << "Finding mutual followers of: ";
//     for (int id : ids) std::cout << id << " ";
//     std::cout << "\n";

//     std::vector<int> mutual = computeMutualFollowers(g, ids);

//     printVector(mutual);

//     return 0;
// }
