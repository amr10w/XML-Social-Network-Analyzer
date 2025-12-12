#include "Suggest.h"
#include <algorithm>
#include <iostream>

std::vector<int> removeValues(const std::vector<int>& source,
                              const std::vector<int>& toRemove)
{
    std::vector<int> result;
    result.reserve(source.size());

    for (int x : source)
    {
        // check if x exists in toRemove (linear search)
        if (std::find(toRemove.begin(), toRemove.end(), x) == toRemove.end())
        {
            result.push_back(x);   // keep it
        }
    }

    return result;
}


std::vector<int> suggest(Graph& graph, int id)
{
    std::vector<int> suggest;
    std::vector<int> followers = graph.getNeighbors(id);
    std::vector<int> followings = graph.getFollowings(id);
    
    for (int i = 0; i < (int)followers.size(); i++)
    {
        std::vector<int> fofollowers = graph.getNeighbors(followers[i]);

        for (int j = 0; j < (int)fofollowers.size(); j++)
            suggest.push_back(fofollowers[j]);
    }

    std::sort(suggest.begin(), suggest.end());
    suggest.erase(std::unique(suggest.begin(), suggest.end()), suggest.end());

    suggest.erase(
        std::remove(suggest.begin(), suggest.end(), id),
        suggest.end()
    );


    return removeValues(suggest,followings);
}

void printSuggestions(const std::vector<int>& v)
{
    if (v.empty())
    {
        std::cout << "No suggestions founddd.\n";
        return;
    }

    std::cout << "Suggested: ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << "\n";
}


// int main()
// {
//     std::string file = "test.xml";
//     std::string xml = readFileToString(file);  

//     Graph g(xml);                              

//     std::vector<int> suggestion = suggest(g,7);
//     printSuggestions(suggestion);
//     return 0;
// }

