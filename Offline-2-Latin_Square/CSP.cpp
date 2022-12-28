#pragma once

#include <iostream>
#include <algorithm>
#include "CSP.h"

CSP::CSP(int n)
{
    this->n = n;
    this->square.resize(n, vector<Slot *>(n));
    this->unassigned_slots_sorted = false;
    Slot::n = n;
}

Slot* CSP::select_unassigned_slot()
{
    if (!unassigned_slots_sorted){
        sort(unassigned_slots.begin(), unassigned_slots.end(), by_legal_value_count());
        unassigned_slots_sorted = true;
    }
    
    Slot *slot_by_heuristic = unassigned_slots[unassigned_slots.size() - 1];
    unassigned_slots.pop_back();
    return slot_by_heuristic;
}

vector<int> CSP::order_domain_values(Slot *unassigned_slot)
{
    // no ordering for now
    return unassigned_slot->get_domain_values();
}

bool CSP::consistent(int value, Slot *unassigned_slot)
{
    int y = unassigned_slot->getY();
    int x = unassigned_slot->getX();
    for (int j = 0; j < n; j++)
        if (square[y][j]->value() == value)
            return false;

    for (int i = 0; i < n; i++)
        if (square[i][x]->value() == value)
            return false;
    return true;
}

bool CSP::assignment_is_complete() {
    return unassigned_slots.size() == 0;
}

void CSP::apply_arc_consistency(Slot* original_slot){
    int y = original_slot->getY();
    int x = original_slot->getX();
    int v = original_slot->value();
    for (int j = 0; j < n; j++)
        square[y][j]->discard_value_from_domain(v);
    for (int i = 0; i < n; i++)
        square[i][x]->discard_value_from_domain(v);
}

void CSP::addVariable(Slot *slot)
{
    int x = slot->getX();
    int y = slot->getY();
    square[y][x] = slot;
    if (!slot->isSet())
        unassigned_slots.push_back(slot);
    else
        assigned_slots.push_back(slot);
}

void CSP::solve()
{
    // adjust domain values for all the variables linked with assigned variables
    for (auto s : assigned_slots)
        apply_arc_consistency(s);

    int ret = solve_Backtrack();
    
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
        if(consistent(value, unassigned_slot)){
            unassigned_slot->value() = value;
            int result = solve_Backtrack();
            if(result != -1)
                return result;
            unassigned_slot->value() = 0;
        }
    }
    unassigned_slots.push_back(unassigned_slot);
    return -1;
}

CSP::~CSP(){

}