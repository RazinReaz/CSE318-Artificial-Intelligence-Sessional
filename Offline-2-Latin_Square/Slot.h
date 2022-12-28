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
    vector<uint8_t> domain;
    int legal_value_count;
public:
    static int n;
    Slot(int x, int y, int val);
    int getX() const;
    int getY() const;
    int& value();
    bool isSet();
    void setN(int n);
    vector<int> get_domain_values();
    void discard_value_from_domain(int value);
    bool operator==(const Slot &other);
    friend ostream &operator<<(ostream &stream, const Slot &slot);
    friend struct by_legal_value_count;
};

ostream &operator<<(ostream &stream, const Slot &slot)
{
    string s = slot.val == 0 ? " " : to_string(slot.val);
    stream << setw(2) << s << " ";
    return stream;
}