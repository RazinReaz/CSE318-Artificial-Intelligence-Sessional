#pragma once

#include <iostream>
#include <vector>
#include "Slot.cpp"

using namespace std;

class CSP
{
    int n;
    vector<vector<Slot *>> square;
    vector<Slot *> unassigned_slots;
    vector<Slot *> assigned_slots;
    bool unassigned_slots_sorted;
    Slot* select_unassigned_slot();
    vector<int> order_domain_values(Slot* unassigned_slot);
    bool consistent(int value, Slot* unassigned_slot);
    bool assignment_is_complete();
    void apply_arc_consistency(Slot* original_slot);
public:
    CSP(int n);
    void addVariable(Slot *slot);
    void solve();
    int solve_Backtrack();
    int solve_ForwardChecking();
    friend ostream &operator<<(ostream &stream, const CSP &csp);
    ~CSP();
};




ostream &operator<<(ostream &stream, const CSP &csp)
{
    stream << "Latin Square dimension: " << csp.n << "x" << csp.n << endl;
    stream << "Board: " << endl;
    for (int i = 0; i < csp.n; i++)
    {
        for (int j = 0; j < csp.n; j++)
        {
            stream << *(csp.square[i][j]) << " ";
        }
        stream << endl;
    }
    return stream;
}