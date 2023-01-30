#include <bits/stdc++.h>
#include"Classes/Graph.cpp"

using namespace std;

void scheduling_problem_solver(string filename, int heuristic)
{
    ifstream file( filename + ".crs");
    vector<Course> courses(1); //the 0th element is a dummy element
    //Getting the number of courses & populating the courses vector
    int n = 0, id, student_count;
    while (file >> id >> student_count){
        n++;
        Course c(id, student_count);
        courses.push_back(c);   //this line of code assumes that the courses are numbered serially from 1
    }
    file.close();

    // initializing the graph
    Graph<Course> g(n, heuristic);

    // populate the adj list from the .stu file
    // this file's nth line contains the courses that student n is enrolled in
    file.open( filename + ".stu");
    string enrolled_courses;
    student_count = 0;
    while (getline(file, enrolled_courses))
    {
        stringstream ss(enrolled_courses);
        int course_id;
        vector<int> ids;
        while(ss >> course_id)
            ids.push_back(course_id);
        g.push_students_enrolled_courses(ids);
        
        for( int i = 0; i<ids.size(); i++){
            for( int j = i-1; j>=0; j--){
                g.addEdge(courses[ids[i]], courses[ids[j]]);
            }
        }
    }
    

    //insert the nodes into the set. They will be automatically ordered
    for (int i = 1; i<courses.size(); i++) {
        g.insertNode(courses[i]);
    }
    cout << "data: " << filename << endl;
    cout << "courses: " << n << endl;
    cout << g;
    g.schedule();
    g.minimize_conflicts(LINEAR);
    g.conflicts_present();
    // cout << "final schedule: " << endl;
    // g.printSchedule();

    g.report();
    file.close();
    return;
}

int main( int argc, char * argv[]) {
    // available heuristics: LARGEST_DEGREE, LARGEST_SATURATION, LARGEST_ENROLLMENT, RANDOM
    
    int heuristic = RANDOM;
    scheduling_problem_solver("inputs/car-f-92", heuristic);
    scheduling_problem_solver("inputs/car-s-91", heuristic);
    scheduling_problem_solver("inputs/kfu-s-93", heuristic);
    scheduling_problem_solver("inputs/tre-s-92", heuristic);
    scheduling_problem_solver("inputs/yor-f-83", heuristic);
    return 0;
}