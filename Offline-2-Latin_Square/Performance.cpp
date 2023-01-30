#pragma once
#include <chrono>
#include <iostream>

class Performance
{
public:
    std::chrono::steady_clock::time_point begin; 
    std::chrono::steady_clock::time_point end;
    int nodes; 
    int bt;   
    Performance();
    void start_time();
    void end_time();
    void report();
};

Performance::Performance() {
    nodes = 0;
    bt = 0;
}

void Performance::start_time(){
    begin = chrono::steady_clock::now();
}
void Performance::end_time()
{
    end = chrono::steady_clock::now();
}
void Performance::report()
{
    auto time_span = static_cast<chrono::duration<double>>(end - begin);
    cout << "Number of nodes = " << nodes << endl;
    cout << "Number of Backtracks = " << bt << endl;
    cout << "Time difference = " << time_span.count() * 1000 << "[ms]" << endl;
}