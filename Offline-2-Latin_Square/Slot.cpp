#pragma once

#include <iostream>
#include <cstdlib>
#include "Slot.h"

using namespace std;

int Slot::n;
int Slot::ordering_heuristic;

Slot::Slot(int x = -1, int y = -1, int val = 0)
{
    this->x = x;
    this->y = y;
    this->val = val;
    this->set = (val != 0);
    if (!set) {
        this->assigned = false;
        this->legal_value_count = n;
    } else {
        this->assigned = true;
        this->legal_value_count = 0;
    }
    this->unassigned_degree = -1;
}

int Slot::getX() const
{
    return this->x;
}

int Slot::getY() const
{
    return this->y;
}

int &Slot::value()
{
    return this->val;
}

bool Slot::isSet()
{
    return this->set;
}

void Slot::set_legal_value_count(int count) {
    if(!this->set)
        this->legal_value_count = count;
}

void Slot::set_unassigned_degree(int count){
    if (!this->set)
        this->unassigned_degree = count;
}

#define VAH1 1
#define VAH2 2
#define VAH3 3
#define VAH4 4
#define VAH5 5

struct by_heuristic
{
    bool operator()(const Slot *a, const Slot *b) const
    {
        double ac, bc;
        bool cmp;
        if (Slot::ordering_heuristic == VAH1)
        { // most constrained variable
            ac = a->legal_value_count;
            bc = b->legal_value_count;
            cmp = ac < bc;
        }
        else if (Slot::ordering_heuristic == VAH2)
        { // most constraining variable
            ac = a->unassigned_degree;
            bc = b->unassigned_degree;
            cmp = ac > bc;
        }
        else if (Slot::ordering_heuristic == VAH3)
        { // VAH1 ties broken by VAH2
            ac = a->legal_value_count;
            bc = b->legal_value_count;
            if(ac == bc){
                ac = a->unassigned_degree;
                bc = b->unassigned_degree;
            }
            cmp = ac < bc;
        }
        else if (Slot::ordering_heuristic == VAH4)
        { // minimum of VAH1/VAH2
            ac = (a->legal_value_count*1.0)/(a->unassigned_degree+1);
            bc = (b->legal_value_count*1.0)/(b->unassigned_degree+1);
            cmp = ac < bc;
        }
        else if (Slot::ordering_heuristic == VAH5)
        { // random
            ac = bc = 0;
        }

        if(ac == bc){
            // tie breaking with slot's x,y
            if(a->x == b->x) return a->y < b->y;
            return a->x < b->x;
        } 
        return cmp;
    }
};