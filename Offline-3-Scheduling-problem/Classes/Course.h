#pragma once

template <typename node>
class Graph;
template <typename node>
class Edge;

class Course
{
private:
    int id;
    int date;
    int degree;
    int student_count;
public:
    Course(int, int);
    ~Course();
    bool operator==(const Course&);
    bool operator<(const Course&);
    friend std::ostream &operator<<(std::ostream &stream, const Course& c);
    int getId() { return this->id; }
    friend class Graph<Course>;
    friend class Edge<Course>;
};
