#include <iostream>
#include <vector>
#include "NeededMath.h"

using namespace std;

#ifndef RAYTRACER_HELPERS_H
#define RAYTRACER_HELPERS_H


/**
 * All surfaces materials struct
 */
struct Material {
    Vec3 ambient, diffuse, specular;
    float shininess;
};

/**
 * Any SceneEntity
 */
class SceneObj {
public:
    Vec3 position = Vec3(0, 0, 0);

    SceneObj() {}

    SceneObj(Vec3 pos) { position = pos; }
};

/**
 * All object that can be rendered respond to ray intersections
 */
class Renderable : public SceneObj {
public:
    Material material;

    Renderable() {}

    Renderable(const Vec3 &pos) : SceneObj(pos) {}

    virtual double intersect(Ray ray){return INFINITY;}
};

/**
 * The camera
 */
class Camera : public SceneObj {
public:
    float fov;
    float focalLength;
    float aspectRatio;

    Camera() {}

    Camera(const Vec3 &pos) : SceneObj(pos) {}

    Camera(const Vec3 &pos, float fov, float fl, float ar) : SceneObj(pos) {
        this->fov = fov;
        this->focalLength = fl;
        this->aspectRatio = ar;
    }
};

/**
 * Lights source
 */
class Light : public SceneObj {
public:
    Vec3 diffuseColor;
    Vec3 specularColor;

    Light() {}
};


/**
 * Spheres
 */
class Sphere : public Renderable {
public:
    double radius;

    Sphere(const Vec3 &pos, double radius) : Renderable(pos), radius(radius) {}

    /**
     * Try to intersect the ray with the shape, and return only the closest t solution.
     * Ignore -t solutions.
     * Returns INFINITY otherwise.
     *
     * @param ray
     * @return t
     */
    double intersect(Ray ray) override {
        Vec3 diff = ray.origin - position;

        double b = 2*(dot(diff, ray.direction.normalize()));
        double c = dot(diff, diff) - pow(radius, 2);

        // Solving quadratic
        double delta = b*b - 4*c;
        double result = INFINITY;

        if(delta == 0){
            result = -b/2;
        } else if (delta > 0) {
            delta = sqrt(delta);
            double t0 = (-b+delta)/2;
            double t1 = (-b-delta)/2;

            // Clamp for smallest over 0
            t0 = t0 < 0 ? INFINITY : t0;
            t1 = t1 < 0 ? INFINITY : t1;
            result = min(t0,t1);
        }

        return result;
    }
};


/**
 * Infinite plane
 */
class Plane : public Renderable {
public:
    Vec3 normal;

    Plane() {}

    double intersect(Ray ray) override {
        return INFINITY;
    }
};


/**
 * Scene containing all objects
 */
struct Scene {
    Camera cam = Camera(Vec3());
    vector<Light *> lights;
    vector<Renderable *> objs;
};




#endif //RAYTRACER_HELPERS_H
