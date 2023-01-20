#pragma once

#include <utility>
#include "Graph.h"

template <typename node>
Graph<node>::Graph(int vertex_count, int heuristic) 
{
    this->vertex_count = vertex_count;
    this->adj.resize(vertex_count+1);
    this->scheduled_vertices.resize(vertex_count+1);
    this->student_courses.resize(1); // the first entry is a dummy
    Course::heuristic = heuristic;
}

template <typename node>
Graph<node>::~Graph()
{
}

template <typename node>
void Graph<node>::addEdge(node& u, node& v)
{
    std::pair<int, int> edge_pair = std::make_pair(u.id, v.id);
    if (!edge_exists[edge_pair])
    {
        u.degree++;
        v.degree++;
        edge_exists[edge_pair] = true;
        adj[u.id].push_back(&v);
        adj[v.id].push_back(&u);
    }
}

template <typename node>
void Graph<node>::insertNode(node& u)
{
    vertices.insert(&u);
}

template <typename node>
node* Graph<node>::get_next_node()
{
    node *current_node = *this->vertices.begin();
    this->vertices.erase(this->vertices.begin());
    return current_node;
}

template <typename node>
void Graph<node>::schedule()
{
    std::vector<int> assigned_dates(vertex_count + 1, 0);
    int imax = -1;
    while(!this->vertices.empty()) {
        node *current_node = get_next_node();
        
        // std::cout << "current node is " << *current_node << std::endl;

        for (int i = 0; i < adj[current_node->id].size(); i++) {
            node *adj_node = adj[current_node->id][i];
            if (adj_node->is_assigned())
                assigned_dates[adj_node->date] = 1;
        }
        // find the first available date and assign it to the current node
        for (int i = 0; i < assigned_dates.size(); i++) {
            if (assigned_dates[i] == 0) {
                // std::cout << "found color " << i << std::endl;
                current_node->date = i;
                imax = std::max(imax, i);
                // std::cout << "current node after assignment " << *current_node << std::endl << std::endl;
                break;
            }
        }

        // if heuristics is saturations, we need to find all the adjacent non-colored\
        nodes of current, set their saturation, and insert them into the set
        if(Course::heuristic == LARGEST_SATURATION) {
            for (int i = 0; i < adj[current_node->id].size(); i++){
                node *adj_node = adj[current_node->id][i];
                
                if(adj_node->is_assigned()) continue;

                auto it = vertices.find(adj_node);
                vertices.erase(it);
                int saturation = calculate_saturation(*adj_node);
                adj_node->saturation = saturation;
                vertices.insert(adj_node);
            }
        }
        scheduled_vertices[current_node->id] = current_node;

        // reset the assigned dates vector
        for (int i = 0; i<assigned_dates.size(); i++) assigned_dates[i] = 0;
    }
    std::cout << "total days " << imax+1 << std::endl;
}

template <typename node>
int Graph<node>::count_set_bits(int mask)
{
    int c = 0;
    while (mask) {
        mask &= (mask - 1);
        c++;
    }
    return c;
}

// ! this function assumes total number of colors is always less than 64
template <typename node>
int Graph<node>::calculate_saturation(node& u)
{
    long long saturation_mask = 0;
    for (int i = 0; i < adj[u.id].size(); i++) {
        node *adj_node = adj[u.id][i];
        if (adj_node->is_assigned()) {
            saturation_mask |= (1 << adj_node->date);
        }
    }
    return count_set_bits(saturation_mask);
}


template <typename node>
double Graph<node>::penaltyLinear() {
    double penalty = 0;
    for (int std_id = 1; std_id < student_courses.size(); std_id++) {
        //for each student
        for (int i = 0; i < student_courses[std_id].size()-1; i++) {
            int course_id_1 = student_courses[std_id][i];
            for (int j = i + 1; j < student_courses[std_id].size(); j++) {
                int course_id_2 = student_courses[std_id][j];
                int gap = abs(scheduled_vertices[course_id_1]->date - scheduled_vertices[course_id_2]->date);
                if(gap == 0) std::cout << "bhairebhai hoynai toh" << std::endl;
                gap <= 5 ? penalty += 2 * ( 5 - gap ) : penalty += 0;
            }
        }
    }

    return penalty / (student_courses.size()-1);
}

template <typename node>
double Graph<node>::penaltyExponential()
{
    double penalty = 0;
    for (int std_id = 1; std_id < student_courses.size(); std_id++)
    {
        // for each student
        for (int i = 0; i < student_courses[std_id].size() - 1; i++)
        {
            int course_id_1 = student_courses[std_id][i];
            for (int j = i + 1; j < student_courses[std_id].size(); j++)
            {
                int course_id_2 = student_courses[std_id][j];
                int gap = abs(scheduled_vertices[course_id_1]->date - scheduled_vertices[course_id_2]->date);
                if (gap == 0)
                    std::cout << "bhairebhai hoynai toh" << std::endl;
                gap <= 5 ? penalty += pow(2.0,(5 - gap)*1.0) : penalty += 0;
            }
        }
    }

    return penalty / (student_courses.size() - 1);
}

template <typename node>
void Graph<node>::printSchedule(){
    for (int i = 0; i < scheduled_vertices.size(); i++) {
        if (scheduled_vertices[i] != nullptr) {
            std::cout << "Course " << i << " was assigned the day " << scheduled_vertices[i]->date << std::endl;
        }
    }
}

template <typename node>
void Graph<node>::print_student_courses() {
    for (int i = 1; i<student_courses.size(); i++) {
        std::cout << "Student " << i << " is taking courses ";
        for (int j = 0; j < student_courses[i].size(); j++) {
            std::cout << student_courses[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
