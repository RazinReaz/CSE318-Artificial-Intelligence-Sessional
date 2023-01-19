#pragma once

#include "Course.h"
#include <iostream>
#include <iomanip>

// the default values are there only to initialize the vector of courses
Course::Course(int id = -1, int student_count = -1)
{
    this->id = id;
    this->date = -1;
    this->degree = 0;
    this->student_count = student_count;
}

Course::~Course()
{
}

bool Course::operator==(const Course &that)
{
    return this->id == that.id;
}

bool Course::operator<(const Course &that)
{
    return this->id < that.id;
}

std::ostream &operator<<(std::ostream &stream, const Course &c)
{
    stream << c.id << "[" << std::setw(3) << c.student_count << "]";
    return stream;
}
