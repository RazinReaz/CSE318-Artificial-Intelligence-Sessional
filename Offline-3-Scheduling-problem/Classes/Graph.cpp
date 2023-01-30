#pragma once

#include <utility>
#include "Graph.h"

template <typename node>
Graph<node>::Graph(int vertex_count, int heuristic) 
{
    this->vertex_count = vertex_count;
    this->timeslots = 0;

    this->initial_penalty = 0;
    this->penalty_after_kempe = 0;
    this->penalty_after_pair_swap = 0;

    this->adj.resize(vertex_count + 1);
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
void Graph<node>::insertNode(node& u) {
    vertices.insert(&u);
}

template <typename node>
node* Graph<node>::get_next_node() {
    node *current_node = *this->vertices.begin();
    this->vertices.erase(this->vertices.begin());
    return current_node;
}

template <typename node>
void Graph<node>::push_students_enrolled_courses(std::vector<int> &course_ids) {
    this->student_courses.push_back(course_ids);
}

template <typename node>
void Graph<node>::schedule() {
    std::vector<int> assigned_dates(vertex_count + 1, 0);
    int imax = -1;

    while(!this->vertices.empty()) {
        node *current_node = get_next_node();
        for (int i = 0; i < adj[current_node->id].size(); i++) {
            node *adj_node = adj[current_node->id][i];
            if (adj_node->is_assigned())
                assigned_dates[adj_node->date] = 1;
        }
        // find the first available date and assign it to the current node
        for (int i = 0; i < assigned_dates.size(); i++) {
            if (assigned_dates[i] == 0) {
                current_node->date = i;
                imax = std::max(imax, i);
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
        scheduled_vertices_ordered.push_back(current_node);
        // reset the assigned dates vector
        for (int i = 0; i<assigned_dates.size(); i++) assigned_dates[i] = 0;
    }

    this->timeslots = imax + 1;
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
double Graph<node>::penalty(int type) {
    double penalty = 0;
    for (int std_id = 1; std_id < student_courses.size(); std_id++) {
        //for each student
        for (int i = 0; i < student_courses[std_id].size()-1; i++) {
            int course_id_1 = student_courses[std_id][i];
            for (int j = i + 1; j < student_courses[std_id].size(); j++) {
                int course_id_2 = student_courses[std_id][j];
                int gap = abs(scheduled_vertices[course_id_1]->date - scheduled_vertices[course_id_2]->date);
                if(gap == 0) std::cout << "bhairebhai coloring hoynai toh" << std::endl;
                double penalty_per_student;
                if ( type == LINEAR ) penalty_per_student = 2*(5-gap);
                else if ( type == EXPONENTIAL) penalty_per_student = pow(2.0, (5-gap)*1.0);
                gap <= 5 ? penalty += penalty_per_student : penalty += 0;
            }
        }
    }
    return penalty / (student_courses.size()-1);
}

template <typename node>
void Graph<node>::minimize_conflicts(int penalty_type) {

    int n = 1000;
    this->initial_penalty = penalty(penalty_type);
    double prev_penalty = initial_penalty;

    while(n--) {
        for ( node* source : scheduled_vertices_ordered ) {
            std::vector<std::pair<int,int>> second_dates = get_second_dates(*source);
            for (std::pair<int, int> second_date_freq : second_dates) {
                int first_date = source->date;
                int second_date = second_date_freq.first;
                kempe_chain_interchange(*source, first_date, second_date);
                double new_penalty = penalty(penalty_type);

                if(new_penalty < prev_penalty) {
                    prev_penalty = new_penalty;
                }
                else {
                    kempe_chain_interchange(*source, second_date, first_date);
                }
                n--;
                if(n <= 0) break;
            }
            if (n <= 0) break;
        }
    }
    this->penalty_after_kempe = prev_penalty;
    //pair swap operator
    n = 1000;
    for(int i = 1; i < scheduled_vertices.size()-1 ; i++) {
        node* course1 = scheduled_vertices[i];
        for(int j=i+1; j<scheduled_vertices.size(); j++) {
            bool continue_flag = false;
            node* course2 = scheduled_vertices[j];
            int date1 = course1->date;
            int date2 = course2->date;

            //find if course1 has adjacent date2, if so, continue j loop
            for (node* adj_node : adj[course1->id]) {
                if(adj_node->date == date2) {
                    continue_flag = true;
                    break;
                }
            }
            if (continue_flag) continue;
            //find if course2 has adjacent date1, if so, continue j loop
            for (node *adj_node : adj[course2->id]) {
                if (adj_node->date == date1) {
                    continue_flag = true;
                    break;
                }
            }
            if (continue_flag) continue;

            course1->date = date2;
            course2->date = date1;
            double new_penalty = penalty(penalty_type);
            if(new_penalty < prev_penalty) {
                prev_penalty = new_penalty;
            }
            else {
                course1->date = date1;
                course2->date = date2;
            }
            n--;
            if(n <= 0) break;
        }
        if(n <= 0) break;
    }
    this->penalty_after_pair_swap = prev_penalty;
    return;
}

template <typename node>
std::vector<std::pair<int, int>> Graph<node>::get_second_dates(node &u)
{
    int date2 = -1;
    std::map<int, int> date_freq;
    for (int i = 0; i < adj[u.id].size(); i++) {
        node *adj_node = adj[u.id][i];
        date_freq[adj_node->date]++;
    }
    std::vector<std::pair<int, int>> date_freq_sorted;
    for (auto it = date_freq.begin(); it != date_freq.end(); it++) {
        date_freq_sorted.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(date_freq_sorted.begin(), date_freq_sorted.end(), 
        [](const std::pair<int, int> a, const std::pair<int, int> b) {
        if(a.second == b.second) return a.first < b.first;
        return a.second > b.second;
    });
    return date_freq_sorted;
}

template <typename node>
void Graph<node>::kempe_chain_interchange(node& u, int date1, int date2) {
    std::queue<node*> q;
    q.push(&u);
    std::vector<bool> visited(vertex_count+1, false);
    visited[u.id] = true;
    while (!q.empty()) {
        node *current_node = q.front();
        q.pop();
        current_node->date == date2 ? current_node->date = date1 : current_node->date = date2;
        for (int i = 0; i < adj[current_node->id].size(); i++) {
            node *adj_node = adj[current_node->id][i];
            if (!visited[adj_node->id] && (adj_node->date == date1 || adj_node->date == date2)){
                visited[adj_node->id] = true;
                q.push(adj_node);
            }
        }
    }
}



template <typename node>
void Graph<node>::printSchedule(){
    for (int i = 0; i < scheduled_vertices.size(); i++) {
        if (scheduled_vertices[i] != nullptr) {
            std::cout << "Course " <<std:: setw(3) << i << " was assigned the day " << scheduled_vertices[i]->date << std::endl;
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


template <typename node>
bool Graph<node>::conflicts_present() {
    bool result = false;
    for (int i = 1; i<vertex_count; i++) {
        for( auto c : adj[i] ) {
            if (scheduled_vertices[i]->date == scheduled_vertices[c->id]->date) {
                result = true;
                std::cout << "conflict between " << i << " and " << c->id << " with date " << scheduled_vertices[i]->date << std::endl;
            }
        }
    }
    return result;
}

template <typename node>
void Graph<node>::report() {
    std::cout << "Timeslots: " << timeslots << std::endl;
    std::cout << "Initial Penalty: " << initial_penalty << std::endl;
    std::cout << "Penalty after Kempe: " << penalty_after_kempe << std::endl;
    std::cout << "penalty after pair-swap " << penalty_after_pair_swap << std::endl;
}