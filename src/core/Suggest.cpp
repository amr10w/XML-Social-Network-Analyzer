#include "Suggest.h"
#include <algorithm>
#include <iostream>


std::vector<int> suggest(Graph& graph, int id)
{
    
    std::vector<int> suggest;

    //get the followers of the user
    std::vector<int> followers = graph.getNeighbors(id);

    //get the followers of followers and add them to suggest
    for (int i = 0; i < (int)followers.size(); i++)
    {
        std::vector<int> fofollowers = graph.getNeighbors(followers[i]);

        for (int j = 0; j < (int)fofollowers.size(); j++)
            suggest.push_back(fofollowers[j]);
    }

    //remove the duplicates
    std::sort(suggest.begin(), suggest.end());
    suggest.erase(std::unique(suggest.begin(), suggest.end()), suggest.end());

    //remove the user itself if appears in the followers of followers
    suggest.erase(
        std::remove(suggest.begin(), suggest.end(), id),
        suggest.end()
    );

    return suggest;
}

void printsuggestions(const std::vector<int>& v)
{
    if (v.empty())
    {
        std::cout << "No suggestions found.\n";
        return;
    }

    std::cout << "Suggested: ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << "\n";
}

int main()
{
    std::string file = "test.xml";
    Graph g(file);

    std::vector<int> suggestion = suggest(g, 1);

    printsuggestions(suggestion);
    return 0;
}
