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
    cout << "Number of courses : " << n << endl;
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
        g.student_courses.push_back(ids);
        
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
    cout << g << endl;
    g.print_student_courses();
    g.schedule();
    g.printSchedule();
    cout << "lnr penalty : " << g.penaltyLinear() << endl;
    cout << "Exp penalty : " << g.penaltyExponential() << endl;
    // for (auto x : courses) cout << x << endl;
    file.close();
    return;
}

int main( int argc, char * argv[]) {
    scheduling_problem_solver("inputs/in", LARGEST_SATURATION);
    return 0;
}