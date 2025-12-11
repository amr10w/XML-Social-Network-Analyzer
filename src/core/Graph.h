#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <map>
#include "FileUtils.h"


typedef std::map<int,std::vector<int>> GRAPH;

class Graph
{
private:
    GRAPH graph;
    int numberOfUsers;
    std::map<int, int> postCounts;
    std::map<int, std::string> names; // mapping ID to name
    // std::vector<Vertex> vertex;
    void buildGraph(const std::vector<Token> &tokens);
    int countUsers(const std::vector<Token> &tokens);
    int getOutdegree(int id);
    int getIndegree(int id);
public:
    Graph(const std::string &content);
    bool addEdge(int from,int to);
    std::vector<int> getNeighbors(int from);
    bool hasEdge(int from,int to);
    void print();
    int getNumberOfUsers();
    GRAPH getGraph() const;
    int getMostInfluencerId();
    std::string getMostInfluencerName();
    int getMostActivePersonId();
    std::string getMostActivePersonName();
    int getMostActivePersonIdPosts();
    std::string getName(int id);
    std::vector<int> getFollowings(int id);
};
#endif
