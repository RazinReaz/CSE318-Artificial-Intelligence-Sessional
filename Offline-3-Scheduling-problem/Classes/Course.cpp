#pragma once

#include "Course.h"
#include "PMrand.h"
#include <iostream>
#include <iomanip>

// the default values are there only to initialize the vector of courses
Course::Course(int id = -1, int student_count = -1)
{
    this->id = id;
    this->date = -1;
    this->degree = 0;
    this->saturation = 0;
    this->student_count = student_count;
    this->random_value = PMrand();
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

bool Course::is_assigned()
{
    return this->date != -1;
}

std::ostream &operator<<(std::ostream &stream, const Course &c)
{
    stream << c.id << "[";
    if(Course::heuristic == LARGEST_DEGREE)     stream << "deg" << std::setw(3) << c.degree;
    if(Course::heuristic == LARGEST_SATURATION) stream << "sat" << std::setw(3) << c.saturation;
    if(Course::heuristic == LARGEST_ENROLLMENT) stream << "enr" << std::setw(3) << c.student_count;
    stream << " <" << std::setw(3) << c.date << "> ";
    stream << "]";
    return stream;
}
