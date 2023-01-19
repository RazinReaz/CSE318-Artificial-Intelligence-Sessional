#pragma once

#include "Graph.h"

template <typename node>
Graph<node>::Graph(int vertex_count) 
{
    this->vertex_count = vertex_count;
    this->adj.resize(vertex_count+1);
    this->vertices.resize(vertex_count+1);
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
    // std::cout << e << std::endl;
    if(!edge_exists[e.getPair()]){
        edge_exists[e.getPair()] = true;
        adj[u.id].push_back(v);
        adj[v.id].push_back(u);
    }
}

template <typename node>
void Graph<node>::addNode(node& u, int index)
{
    vertices[index] = u;
}

template <typename node>
void Graph<node>::setVertices(std::vector<node>& vertices)
{
    this->vertices = vertices;
}

template <typename node>
std::vector<node> Graph<node>::getVertices()
{
    return this->vertices;
}