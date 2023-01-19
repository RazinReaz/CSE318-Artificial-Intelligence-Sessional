#pragma once

#include "Graph.h"

template <typename node>
Graph<node>::Graph(int vertex_count, int heuristic) 
{
    this->vertex_count = vertex_count;
    this->adj.resize(vertex_count+1);
    Course::heuristic = heuristic;
}

template <typename node>
Graph<node>::~Graph()
{
}

template <typename node>
void Graph<node>::addEdge(node& u, node& v)
{
    //create the edge
    Edge<node> e(u,v);
    if(!edge_exists[e.getPair()]){
        edge_exists[e.getPair()] = true;
        adj[u.id].push_back(v);
        adj[v.id].push_back(u);
        u.degree++;
        v.degree++;
    }
}

template <typename node>
void Graph<node>::insertNode(node& u)
{
    vertices.insert(u);
}

template <typename node>
std::vector<node> Graph<node>::getVertices()
{
    return this->vertices;
}