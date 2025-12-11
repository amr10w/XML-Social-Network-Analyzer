#include "Graph.h"

Graph::Graph(const std::string &content)
{
    
    std::vector<Token> tokens= tokenizeXML(content);
    int users =countUsers(tokens);
    numberOfUsers=users;
    buildGraph(tokens);
}


void Graph::buildGraph(const std::vector<Token> &tokens)
{
    for(int i=0;i<tokens.size();i++)
    {
        if(tokens[i].type=="tag" &&tokens[i].text == "id" )
        {
            i++;
            int id;
            if(i < tokens.size()&&tokens[i].type=="value"){
                try
                {
                    id = std::stoi(tokens[i].text);
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << "Invalid user ID: " << tokens[i].text << "\n";
                    continue; 
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << "User ID out of range: " << tokens[i].text << "\n";
                    continue;
                }
            }
            else
            {
                std::cerr << "Missing value for user ID at token " << i << "\n";
                continue;
            }

            if (graph.find(id) == graph.end()) {
                graph[id] = {};
            }
            
            int currentPostCount = 0; // Initialize counter for the current user

            while(i < tokens.size()&&tokens[i].text != "/user")
            {
                // NEW: Count posts for the current user ID
                if(tokens[i].type=="tag" &&tokens[i].text == "post" )
                {
                    currentPostCount++;
                }
                
                if(tokens[i].type=="tag" &&tokens[i].text == "id" )
                {
                    i++;
                    int follow;
                    if(tokens[i].type=="value"){
                        try
                        {
                            int follow = std::stoi(tokens[i].text);
                            if (id >= 0)
                                graph[id].push_back(follow);
                        }
                        catch (const std::invalid_argument &e)
                        {
                            std::cerr << "Invalid follow ID: " << tokens[i].text << "\n";
                        }
                        catch (const std::out_of_range &e)
                        {
                            std::cerr << "Follow ID out of range: " << tokens[i].text << "\n";
                        }
                    }
                    else
                    {
                        std::cerr << "Missing value for follow ID at token " << i << "\n";
                    }
                }
                i++;
            }
            
            // NEW: Store the total post count after processing the user's block
            postCounts[id] = currentPostCount;
        }
    }
}

int Graph::countUsers(const std::vector<Token> &tokens)
{
    int count=0;
    
    for(int i=0;i<tokens.size();i++)
    {
        if(tokens[i].type=="tag" &&tokens[i].text == "user" )
        {
            count++;
        }
    }
    return count;
}

void Graph::print()
{
    for (const auto& entry : graph)
    {
        std::cout << entry.first << ": ";

        for (int neighbor : entry.second)
        {
            std::cout << neighbor << " ";
        }

        std::cout << "\n";
    }
}

bool Graph::addEdge(int from, int to)
{
    if(graph.find(from) != graph.end()){
        graph[from].push_back(to);
        return true;
    }
    return false;
}

std::vector<int> Graph::getNeighbors(int from)
{
    if(graph.find(from) != graph.end())
        return graph[from];
    return {};
}

bool Graph::hasEdge(int from,int to)
{
    if(graph.find(from) == graph.end())
        return false;
    for(int neighbor : graph[from])
    {
        if(neighbor == to)
            return true;
    }
    return false;
}

int Graph::getNumberOfUsers()
{
    return graph.size();
}

GRAPH Graph::getGraph() const
{
    return graph;
}

int Graph::getMostInfluencerId()
{
    if (graph.empty()) {
        return -1;
    }

    int mostInfluencerId = -1;
    size_t maxFollowers = 0;

    for (const auto& entry : graph)
    {
        int userId = entry.first;
        const std::vector<int>& followers = entry.second;
        
        if (followers.size() > maxFollowers)
        {
            maxFollowers = followers.size();
            mostInfluencerId = userId;
        }
    }

    if (mostInfluencerId == -1 && !graph.empty()) {
        return graph.begin()->first;
    }

    return mostInfluencerId;
}
int Graph::getMostActivePersonId()
{
    if (graph.empty()) {
        return -1;
    }
    std::map<int, int> followingCounts;
    for (const auto& entry :  graph) {
        followingCounts[entry.first] = 0;
    }
    for (const auto& entry:graph) {
        for (int follower:entry.second) {
            followingCounts[follower]++;
        }
    }
    for (const auto& entry:graph) {
        
        followingCounts[entry.first]+=entry.second.size();
        
    }
    int mostActiveId = -1;
    int maxFollowing = -1;
    for (const auto& entry:followingCounts) {
        if (entry.second > maxFollowing) {
            maxFollowing = entry.second;
            mostActiveId = entry.first;
        }
    }
    return mostActiveId;
}
int Graph::getMostActivePersonIdPosts()
{
    if (postCounts.empty()) {
        return -1;
    }

    int mostActiveId = -1;
    int maxPosts = -1;

    for (const auto& entry : postCounts)
    {
        int userId = entry.first;
        int postCount = entry.second;
        
        if (postCount > maxPosts)
        {
            maxPosts = postCount;
            mostActiveId = userId;
        }
    }

    return mostActiveId;
}


