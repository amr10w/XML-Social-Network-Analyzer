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
                    continue; // Skip this user
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
            // Ensure user exists in graph even if they have no followers
            if (graph.find(id) == graph.end()) {
                graph[id] = {};
            }

            while(i < tokens.size()&&tokens[i].text != "/user")
            {
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
