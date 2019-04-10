
#ifndef RAYTRACER_NEEDEDMATH_H
#define RAYTRACER_NEEDEDMATH_H

#include <sstream>
using namespace std;

/**
 * Vector definition
 */
struct Vec3 {
    double x, y, z;

    Vec3(){x=y=z=0;}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator + (const Vec3& v) const { return {x+v.x, y+v.y, z+v.z}; }
    Vec3 operator - (const Vec3& v) const { return {x-v.x, y-v.y, z-v.z}; }
    Vec3 operator * (double d) const { return {x*d, y*d, z*d}; }
    Vec3 operator / (double d) const { return {x/d, y/d, z/d}; }

    Vec3 normalize() const {
        double mg = sqrt(x*x + y*y + z*z);
        return {x/mg,y/mg,z/mg};
    }

    string toString() const {
        return "{" + to_string(x) + " " + to_string(y) + " " + to_string(z) + "}";
    }
};

inline double dot(const Vec3& a, const Vec3& b) {
    return (a.x*b.x + a.y*b.y + a.z*b.z);
}


/**
 * Rays
 */
struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction) {}
};


#endif //RAYTRACER_NEEDEDMATH_H
