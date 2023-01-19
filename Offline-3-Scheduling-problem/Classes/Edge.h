#pragma once

#include <iostream>
#include <utility>
#include "Course.cpp"

template <typename node>
class Edge
{
    node u;
    node v;

public:
    Edge(node U, node V) : u(U), v(V) {}
    ~Edge() {}
    bool operator==(const Edge<node> &that) { return this->u == that.u && this->v == that.v; }
    bool operator<(const Edge<node> &that)
    {
        if (this->u == that.u)
            return this->v < that.v;
        return this->u < that.u;
    }
    std::pair<int, int> getPair() { return std::make_pair(u.id, v.id); }
    template <typename _node>
    friend std::ostream &operator<<(std::ostream &stream, const Edge<_node> &e);
};

template <typename node>
std::ostream &operator<<(std::ostream &stream, const Edge<node> &e)
{
    stream << "( " << e.u << "-" << e.v << " )";
    return stream;
}