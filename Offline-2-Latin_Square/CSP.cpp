#pragma once

#include <iostream>
#include <algorithm>
#include "CSP.h"

#define FAILURE -1

// assuming n<32
CSP::CSP(int n, int vahnum, bool forward_check)
{
    this->n = n;
    this->forward_check = forward_check;
    this->square.resize(n, vector<Slot *>(n));
    this->row_legal_masks.resize(n);
    this->col_legal_masks.resize(n);
    for(int i = 0; i < n; i++){
        row_legal_masks[i].push((1 << (n + 1)) - 2);
        col_legal_masks[i].push((1 << (n + 1)) - 2);
    }
    Slot::n = n;
    Slot::ordering_heuristic = vahnum;
}

inline void CSP::setbit(int& n, int i){
    n |= (1<<i);
}
inline void CSP::resetbit(int& n, int i){
    n &= ~(1<<i);
}

int CSP::count_set_bits(int mask) {
    int c = 0;
    while(mask){
        mask &= (mask-1);
        c++;
    }
    return c;
}

Slot* CSP::select_unassigned_slot()
{
    Slot* slot_by_heuristic = *(this->unassigned_slots.begin());
    unassigned_slots.erase(unassigned_slots.begin());
    return slot_by_heuristic;
}

vector<int> CSP::order_domain_values(Slot *unassigned_slot)
{
    // no ordering for now
    vector<int> values;
    int x = unassigned_slot->getX();
    int y = unassigned_slot->getY();
    int mask = row_legal_masks[y].top() & col_legal_masks[x].top();
    int i=1;
    while(mask){
        mask >>= 1;
        if(mask & 1)
            values.push_back(i);
        i++;
    }
    return values;
}

bool CSP::consistent(int value, Slot *unassigned_slot)
{
    int y = unassigned_slot->getY();
    int x = unassigned_slot->getX();
    if(forward_check){
        int m = row_legal_masks[y].top() & col_legal_masks[x].top();
        return (m & (1<<value));
    }
    for(int i = 0; i < n; i++)
        if(square[i][x]->value() == value)
            return false;
    for(int j = 0; j < n; j++)
        if(square[y][j]->value() == value)
            return false;
    return true;
}

bool CSP::assignment_is_complete() {
    return unassigned_slots.size() == 0;
}

bool CSP::domain_is_empty(Slot *s, int y, int x)
{
    int r = row_legal_masks[y].top();
    int c = col_legal_masks[x].top();
    return (r&c) == 0;
}

void CSP::apply_arc_consistency(Slot* original_slot){
    int x = original_slot->getX();
    int y = original_slot->getY();

    for(int i = 0; i<n; i++)
        set_heuristic_field(square[i][x], i, x);
    for(int j = 0; j<n; j++)
        set_heuristic_field(square[y][j], y, j);
}

// sets the field to calculate the heuristic of a single slot
void CSP::set_heuristic_field(Slot *s, int y, int x) {
    if (Slot::ordering_heuristic == VAH1 || Slot::ordering_heuristic == VAH3 || Slot::ordering_heuristic == VAH4)
    {
        int mask = row_legal_masks[y].top() & col_legal_masks[x].top();
        int legal_value_count = count_set_bits(mask);
        s->set_legal_value_count(legal_value_count);
    }
    if (Slot::ordering_heuristic == VAH2 || Slot::ordering_heuristic == VAH3 || Slot::ordering_heuristic == VAH4)
    {
        int count = count_set_bits(col_legal_masks[x].top()) + count_set_bits(row_legal_masks[y].top()) - 2;
        s->set_unassigned_degree(count);
    }
}

void CSP::addVariable(Slot *slot)
{
    int x = slot->getX();
    int y = slot->getY();
    int v = slot->value();

    square[y][x] = slot;
    if (slot->isSet()) {
        resetbit(row_legal_masks[y].top(), v);
        resetbit(col_legal_masks[x].top(), v);
    }
}

void CSP::solve()
{
    cout << "solving started" << endl;
    // adjust domain values for all the variables linked with assigned variables
    for(int i = 0; i<n; i++)
        for( int j = 0; j<n; j++)
            if(square[i][j]->isSet())
                apply_arc_consistency(square[i][j]);
    // now insert the unassigned variables into the set
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (!square[i][j]->isSet())
                unassigned_slots.insert(square[i][j]);
    cout << "set size " << unassigned_slots.size() << endl;
    int ret;
    performance.start_time();
    if(forward_check)
        ret = solve_ForwardChecking();
    else 
        ret = solve_Backtrack();
    performance.end_time();

    string result = (ret < 0) ? "csp failed" : "csp solved"; 
    cout << result << endl;
}




int CSP::solve_Backtrack()
{
    if(assignment_is_complete())
        return 0;
    
    Slot* unassigned_slot = select_unassigned_slot();
    vector<int> domain_values = order_domain_values(unassigned_slot);
    for(auto value : domain_values){
        performance.nodes++;
        if(consistent(value, unassigned_slot)){
            unassigned_slot->value() = value;
            performance.bt++;
            int result = solve_Backtrack();
            if (result != FAILURE)
                return result;
            unassigned_slot->value() = 0;
        }
    }
    unassigned_slots.insert(unassigned_slot);
    return FAILURE;
}






int CSP::solve_ForwardChecking()
{
    if (assignment_is_complete())
        return 0;

    Slot *unassigned_slot = select_unassigned_slot();
    vector<int> domain_values = order_domain_values(unassigned_slot);
    for (auto value : domain_values)
    {
        performance.nodes++;
        if (consistent(value, unassigned_slot))
        {
            unassigned_slot->value() = value;
            unassigned_slot->assigned = true;
            bool success = infer_from_slot(unassigned_slot);
            if(success){
                performance.bt++;
                int result = solve_ForwardChecking();
                if (result != FAILURE)
                    return result;
                remove_inferences(unassigned_slot);
            }
            unassigned_slot->value() = 0;
            unassigned_slot->assigned = false;
        }
    }
    unassigned_slots.insert(unassigned_slot);
    return FAILURE;
}

bool CSP::infer_from_slot(Slot *slot)
{
    int x = slot->getX();
    int y = slot->getY();
    int v = slot->value();
    int r = row_legal_masks[y].top();
    int c = col_legal_masks[x].top();
    
    resetbit(r,v);
    resetbit(c,v);

    row_legal_masks[y].push(r);
    col_legal_masks[x].push(c);

    //! Detect failure early
    for (int i = 0; i < n; i++){
        if (i == y || square[i][x]->assigned)
            continue;
        if (domain_is_empty(square[i][x], i, x)){
            // cout << "the culprit was " << *(square[i][x]) << endl;
            row_legal_masks[y].pop();
            col_legal_masks[x].pop();
            return false;
        }
    }
    for (int j = 0; j < n; j++) {
        if (j == x || square[y][j]->assigned)
            continue;
        if (domain_is_empty(square[y][j], y, j)){
            // cout << "the culprit was " << *(square[y][j]) << endl;
            row_legal_masks[y].pop();
            col_legal_masks[x].pop();
            return false;
        }
    }

    // maintaining the set for variable ordering
    for (int i = 0; i < n; i++) {
        if (i == y || square[i][x]->assigned) continue;
        auto it = unassigned_slots.find(square[i][x]);
        unassigned_slots.erase(it);
        set_heuristic_field(square[i][x], i, x);
        unassigned_slots.insert(square[i][x]);
    }
    for (int j = 0; j < n; j++) {
        if (j == x || square[y][j]->assigned) continue;
        auto it = unassigned_slots.find(square[y][j]);
        unassigned_slots.erase(it);
        set_heuristic_field(square[y][j], y, j);
        unassigned_slots.insert(square[y][j]);
    }
    return true;
}

void CSP::remove_inferences(Slot *slot)
{
    int x = slot->getX();
    int y = slot->getY();
    row_legal_masks[y].pop();
    col_legal_masks[x].pop();
    for (int i = 0; i < n; i++){
        if (i == y || square[i][x]->assigned)
            continue;
        auto it = unassigned_slots.find(square[i][x]);
        unassigned_slots.erase(it);
        set_heuristic_field(square[i][x], i, x);
        unassigned_slots.insert(square[i][x]);
    }
    for (int j = 0; j < n; j++){
        if (j == x || square[y][j]->assigned)
            continue;
        auto it = unassigned_slots.find(square[y][j]);
        unassigned_slots.erase(it);
        set_heuristic_field(square[y][j], y, j);
        unassigned_slots.insert(square[y][j]);
    }
}

CSP::~CSP(){

}