#include <bits/stdc++.h>
#include "CSP.cpp"
using namespace std;

int main() {
    ifstream file("inputs\\d-15-01.txt");
    // ifstream file("in.txt");
    int n, value;
    file >> n;

    CSP csp(n);
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            file >> value;
            Slot *slot = new Slot(j,i,value);
            csp.addVariable(slot);
        }
    }
    cout << csp << endl;
    chrono::steady_clock clock;
    auto start = clock.now();

    csp.solve();
    
    auto end = clock.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);

    cout << csp;
    cout << "Time taken : " << time_span.count()*1000 << "ms" << endl;
    file.close();
    return 0;
}