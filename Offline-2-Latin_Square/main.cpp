#include <bits/stdc++.h>
#include "CSP.cpp"
using namespace std;

int main(int argc, char* argv[]) {
    ifstream file("inputs\\d-10-06.txt");
    // ifstream file("in.txt");
    int n, value;
    file >> n;

    CSP csp(n, VAH5, false);
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            file >> value;
            Slot *slot = new Slot(j,i,value);
            csp.addVariable(slot);
        }
    }
    csp.solve();
    cout << csp;
    csp.performance.report();
    file.close();
    return 0;
}