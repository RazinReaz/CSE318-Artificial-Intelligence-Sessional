#include <bits/stdc++.h>
#include"Classes/Graph.cpp"

using namespace std;

int main() {
    ifstream file("inputs/car-f-92.crs");

    
    //Getting the number of courses
    int n = 0, id, student_count;
    while (file >> id >> student_count)
        n++;
    cout << n << endl;

    // initializing the graph
    Graph<Course> g(n, LARGEST_DEGREE);
    vector<Course> courses(n+1);


    //resetting the file to read from beginning again
    file.clear();
    file.seekg(0);


    // populating the courses vector
    while (file >> id >> student_count){
        Course c(id, student_count);
        courses[id] = c;
    }
    file.close();
    // populate the adj list from the .stu file
    // this file's nth line contains the courses that student n is enrolled in
    file.open("inputs/car-f-92.stu");
    string enrolled_courses;
    while (getline(file, enrolled_courses))
    {
        stringstream ss(enrolled_courses);
        int course_id;
        vector<int> ids;
        while(ss >> course_id){
            ids.push_back(course_id);
        }
        for( int i = 0; i<ids.size(); i++){
            for( int j = i-1; j>=0; j--){
                g.addEdge(courses[ids[i]], courses[ids[j]]);
            }
        }
    }

    for ( auto x: courses) {
        g.insertNode(x);
    }
    
    cout << g << endl;
    file.close();
    return 0;
}