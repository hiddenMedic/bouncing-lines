#ifndef BLINES_H
#define BLINES_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
    return degrees * pi / 180.0;
}

// [0, 1)
inline double random_double(){
    return rand() / (RAND_MAX + 1.0);
}

/*
#include <random>

// slower
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
*/

// [min, max)
inline double random_double(double min, double max){
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max){
    return static_cast<int>(random_double(min, max + 1));
}

#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif