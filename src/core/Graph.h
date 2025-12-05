#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include "FileUtils.h"


typedef std::vector<std::vector<int>> GRAPH;

class Graph
{
private:
    GRAPH graph;
    // std::vector<Vertex> vertex;
    void buildGraph(const std::vector<Token> &tokens);
    int countUsers(const std::vector<Token> &tokens);
public:
    Graph(const std::string &content);
    bool addEdge(int from,int to);
    std::vector<int> getNeighbors(int from);
    bool hasEdge(int from,int to);
    void print();
    int getNumberOfUsers();
    GRAPH getGraph() const;

};
#endif