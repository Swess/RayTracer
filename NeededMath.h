
#ifndef RAYTRACER_NEEDEDMATH_H
#define RAYTRACER_NEEDEDMATH_H

#include <sstream>

using namespace std;
using namespace glm;

inline double dot(const vec3 &a, const vec3 &b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

/**
 * Compute the reflection vector around the normal vector
 * @param dir
 * @param normal
 * @return
 */
vec3 reflect(vec3 const &dir, vec3 const &normal){
    return dir - (float)(2*dot(normal, dir))*normal;
}

/**
 * Limit the values of Vec3 color between [0.255]
 * @param color
 */
void clampColor(vec3 &color){
    color.x = (color.x > 255) ? 255 : (color.x < 0) ? 0 : color.x;
    color.y = (color.y > 255) ? 255 : (color.y < 0) ? 0 : color.y;
    color.z = (color.z > 255) ? 255 : (color.z < 0) ? 0 : color.z;
}


/**
 * Rays
 */
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(const vec3 &origin, const vec3 &direction) : origin(origin), direction(direction) {}
};


#endif //RAYTRACER_NEEDEDMATH_H
