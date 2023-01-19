#pragma once

#include "Course.h"
#include <string>

#define LARGEST_DEGREE 1
#define LARGEST_SATURATION 2
#define LARGEST_ENROLLMENT 3

std::string heuristic_names[] = {"", "LARGEST_DEGREE", "LARGEST_SATURATION", "LARGEST_ENROLLMENT"};

struct by_heuristics
{
    template <typename node>
    bool operator()(const node &a, const node &b) const
    {
        int ac = 0, bc = 0;
        bool cmp;
        if(Course::heuristic == LARGEST_DEGREE){
            ac = a.degree;
            bc = b.degree;
            cmp = ac > bc;
        }
        else if(Course::heuristic == LARGEST_SATURATION){
            ac = a.saturation;
            bc = b.saturation;
            cmp = ac > bc;
        }
        else if(Course::heuristic == LARGEST_ENROLLMENT){
            ac = a.student_count;
            bc = b.student_count;
            cmp = ac > bc;
        }

        if(ac == bc)
            return a.id < b.id;
        return cmp;
    }
};