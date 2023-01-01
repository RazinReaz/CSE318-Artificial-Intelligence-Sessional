#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class Slot
{
    int val;
    int x;
    int y;
    bool set;
    int legal_value_count;
    int unassigned_degree;
public:
    static int n;
    static int ordering_heuristic;
    bool assigned;

    Slot(int x, int y, int val);
    int getX() const;
    int getY() const;
    int& value();
    bool isSet();
    vector<int> get_domain_values();
    void set_legal_value_count(int count);
    void set_unassigned_degree(int count);
    bool operator==(const Slot &other);
    friend ostream &operator<<(ostream &stream, const Slot &slot);
    friend struct by_heuristic;
    friend struct by_legal_value_count;
};

ostream &operator<<(ostream &stream, const Slot &slot)
{
    string s = slot.val == 0 ? " " : to_string(slot.val);
    stream << "[" << setw(2) << s << "] ";
    // stream << setw(2) << s << " " << slot.set;
    // stream << setw(2) << s << " " << slot.y << "," << slot.x <<" "<< slot.legal_value_count;
    // stream << "[" << setw(2) << s << "," << setw(2) << slot.legal_value_count << "]";
    return stream;
}