#include <bits/stdc++.h>
#include "CSP.cpp"
using namespace std;

void do_csp(string filename, int vah, bool fc)
{
    ifstream file(filename);
    // ifstream file("in.txt");
    int n, value;
    file >> n;

    CSP csp(n, vah, fc);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            file >> value;
            Slot *slot = new Slot(j, i, value);
            csp.addVariable(slot);
        }
    }
    // cout << csp;
    csp.solve();
    // cout << csp;
    csp.performance.report();
    file.close();
    return;
}

int main(int argc, char* argv[]) {

    do_csp("inputs\\d-15-01.txt", VAH1, true);

    return 0;
}
