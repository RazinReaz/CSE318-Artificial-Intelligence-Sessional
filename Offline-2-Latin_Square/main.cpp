#include <bits/stdc++.h>
#include "CSP.cpp"
using namespace std;

void do_csp(string filename, int heuristic, bool forwardcheck)
{
    ifstream file(filename);
    int n, value;
    file >> n;

    CSP csp(n, heuristic, forwardcheck);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            file >> value;
            Slot *slot = new Slot(j, i, value);
            csp.addVariable(slot);
        }
    }
    cout << "\nBEFORE SOLVING :\n"<< csp;
    csp.solve();
    cout << "\nAFTER SOLVING :\n"<<csp;
    csp.performance.report();
    file.close();
    return;
}

int main(int argc, char* argv[]) {

    do_csp("inputs\\d-10-01.txt", VAH5, true);
    // do_csp("inputs\\d-10-01.txt", VAH2, true);
    // do_csp("inputs\\d-10-01.txt", VAH3, true);
    // do_csp("inputs\\d-10-01.txt", VAH4, true);
    // do_csp("inputs\\d-10-01.txt", VAH5, true);

    return 0;
}
