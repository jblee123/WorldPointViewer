#pragma once

#include <cmath>
#include <cstdlib>

const static double PI = 3.14159265358979323846;
const static float RAD_PER_CIRCLE = 2.0f * (float)PI;
const static float DEG_PER_CIRCLE = 360.0f;

//const static double EARTH_EQUITORIAL_RADIUS = 6378137;
const static double EARTH_EQUITORIAL_RADIUS = 6356752;

inline float randf() {
    return ((float)rand() / (float)RAND_MAX);
}

inline float randAngle() {
    return randf() * RAD_PER_CIRCLE;
}

inline float degToRad(float deg) {
    return (deg / DEG_PER_CIRCLE) * RAD_PER_CIRCLE;
}
