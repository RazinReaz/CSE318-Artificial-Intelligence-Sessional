#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include "Edge.h"

template <typename node>
class Graph
{
private:    
    int vertex_count;
    int edge_count;
    std::vector<std::vector<node>> adj;
    std::vector<node> vertices;
    std::map<std::pair<int, int>, bool> edge_exists; //this map ensures that two nodes don't have multiple edges between them
public:
    Graph(int vertex_count);
    ~Graph();
    void addEdge(node& u, node& v);
    void addNode(node& u, int index);
    void setVertices(std::vector<node>& vertices);
    std::vector<node> getVertices();
};

