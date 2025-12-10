#ifndef MUTUAL_H
#define MUTUAL_H

#include <vector>
#include <string>
#include "Graph.h"

// Computes mutual followers between a list of user IDs
std::vector<int> computeMutualFollowers(Graph& graph, const std::vector<int>& ids);

// Helper to print vector<int>
void printVector(const std::vector<int>& v);

#endif
