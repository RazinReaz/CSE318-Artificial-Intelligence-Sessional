#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include "Comparators.cpp"
#include "Edge.h"

#define LINEAR 0
#define EXPONENTIAL 1

int Course::heuristic;
template <typename node>
class Graph
{
private:
    int vertex_count;
    std::vector<std::vector<node *>> adj;
    std::set<node *, by_heuristics> vertices;
    std::map<std::pair<int, int>, bool> edge_exists; // this map ensures that two nodes don't have multiple edges between them
    std::vector<node *> scheduled_vertices;
    std::vector<node *> scheduled_vertices_ordered; // this vector stores the scheduled vertices in the order they were popped
    std::vector<std::vector<int>> student_courses;  // this vector stores the ith student's enrolled courses
    node *get_next_node();
    int calculate_saturation(node &u);
    int count_set_bits(int mask);
    std::vector<std::pair<int, int>> get_second_dates(node &u);
    void kempe_chain_interchange(node &src, int date1, int date2);

    int timeslots;
    double initial_penalty;
    double penalty_after_kempe;
    double penalty_after_pair_swap;

public:
    Graph(int vertex_count, int heuristic);
    ~Graph();
    void addEdge(node &u, node &v);
    void insertNode(node &u);
    void push_students_enrolled_courses(std::vector<int> &course_ids);
    void schedule();
    void printSchedule();
    void print_student_courses();
    void minimize_conflicts(int penalty_type);
    bool conflicts_present();
    double penalty(int type);
    void report();
    template <typename _node>
    friend std::ostream &operator<<(std::ostream &stream, const Graph<_node> &g);
};

template <typename node>
std::ostream &operator<<(std::ostream &stream, const Graph<node> &g)
{
    stream << "::::::::Graph::::::::" << std::endl;
    stream << "Heuristic used: " << heuristic_names[Course::heuristic] << std::endl;
    // stream << "Number of vertices: " << g.vertex_count << std::endl;
    // stream << "Adjacency list:" << std::endl;
    // for (int i = 1; i <= g.vertex_count; i++) {
    //     stream << i << ": ";
    //     for (int j = 0; j < g.adj[i].size(); j++)
    //     {
    //         stream << *g.adj[i][j] << " -> ";
    //     }
    //     stream << std::endl;
    // }
    return stream;
}
