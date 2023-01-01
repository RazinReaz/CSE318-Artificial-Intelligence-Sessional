#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <string>
#include <bitset>
#include "Slot.cpp"
#include "Performance.cpp"

using namespace std;

class CSP
{
    int n;
    bool forward_check;
    vector<vector<Slot *>> square;
    set<Slot*, by_heuristic> unassigned_slots;
    vector<stack<int>> row_legal_masks;
    vector<stack<int>> col_legal_masks;
    
    inline void setbit(int& n, int i);
    inline void resetbit(int& n, int i);

    int count_set_bits(int mask);
    Slot* select_unassigned_slot();
    vector<int> order_domain_values(Slot* unassigned_slot);
    bool consistent(int value, Slot* unassigned_slot);
    bool assignment_is_complete();
    bool domain_is_empty(Slot* s, int y, int x);
    void apply_arc_consistency(Slot* s);
    void set_heuristic_field(Slot *s, int y, int x);
    int solve_Backtrack();
    int solve_ForwardChecking();
    bool infer_from_slot(Slot* slot);
    void remove_inferences(Slot* slot);
public:
    Performance performance;
    CSP(int n, int vahnum, bool forward_check);
    void addVariable(Slot *slot);
    void solve();
    friend ostream &operator<<(ostream &stream, const CSP &csp);
    ~CSP();
};




ostream &operator<<(ostream &stream, const CSP &csp)
{
    stream << endl;
    stream << "Dimension:\t\t" << csp.n << "x" << csp.n << endl;
    stream << "Heuristics used : \tVAH" <<Slot::ordering_heuristic << endl;
    string u = csp.forward_check ? "Forward check" :"Backtrack";
    stream << "used \t\t\t" << u << endl;
    stream << "Board: " << endl;
    bitset<15> x;
    for (int i = 0; i < csp.n; i++)
    {
        for (int j = 0; j < csp.n; j++)
        {
            stream << *(csp.square[i][j]) << " ";
        }
        // x = csp.row_legal_masks[i].top();
        // stream << "\t" << x;
        stream << endl;
    }
    stream << endl;
    return stream;
}