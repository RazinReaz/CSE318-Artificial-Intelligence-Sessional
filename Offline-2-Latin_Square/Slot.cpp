#pragma once

#include <iostream>
#include "Slot.h"

using namespace std;

int Slot::n;

Slot::Slot(int x = -1, int y = -1, int val = 0) {
    this->x = x;
    this->y = y;
    this->val = val;
    this->set = (val != 0);

    if(!set){
        this->domain.resize(n+1);
        for(int i=1; i<=n; i++) domain[i] = 1;
        this->legal_value_count = n;
    } else {
        this->legal_value_count = 0;
    }
    //cout << x << y << legal_value_count << set << endl;
}

int Slot::getX() const{
    return this->x;
}

int Slot::getY() const{
    return this->y;
}

int& Slot::value() {
    return this->val;
}

bool Slot::isSet() {
    return this->set;
}

void setN(int n) {
    Slot::n = n;
}

vector<int> Slot::get_domain_values()
{
    vector<int> domain_values;
    for(int i=1; i<=n ; i++)
        if(domain[i]==1)
            domain_values.push_back(i);
    return domain_values;
}

void Slot::discard_value_from_domain(int value)
{
    if(!this->isSet() && this->domain[value] != 0){
        this->domain[value] = 0;
        legal_value_count--;
    }
}

bool Slot::operator==(const Slot& other){
    return this->x == other.y && this->x == other.y;
}




// functor for sorting all unset slot pointers by number of legal values
// most constrained variable
struct by_legal_value_count {
    bool operator()(const Slot* a, const Slot* b) {
        return a->legal_value_count > b->legal_value_count;
    }
};