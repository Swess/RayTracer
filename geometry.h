#include <iostream>
#include <vector>
#include "NeededMath.h"

using namespace std;
using namespace glm;

#ifndef RAYTRACER_HELPERS_H
#define RAYTRACER_HELPERS_H


/**
 * All surfaces materials struct
 */
struct Material {
    vec3 ambient, diffuse, specular;
    float shininess;
};

/**
 * Any SceneEntity
 */
class SceneObj {
public:
    vec3 position = vec3();

    SceneObj() {}

    SceneObj(vec3 pos) { position = pos; }
};

/**
 * All object that can be rendered respond to ray intersections
 */
class Renderable : public SceneObj {
public:
    Material material;

    Renderable() {}

    Renderable(const vec3 &pos) : SceneObj(pos) {}

    virtual double intersect(Ray ray){return INFINITY;}
    virtual vec3 getNormalAt(vec3 point){return vec3();}
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

    Camera(const vec3 &pos) : SceneObj(pos) {}

    Camera(const vec3 &pos, float fov, float fl, float ar) : SceneObj(pos) {
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
    vec3 diffuseColor;
    vec3 specularColor;

    Light() {}
};


/**
 * Spheres
 */
class Sphere : public Renderable {
public:
    double radius;

    Sphere(const vec3 &pos, double radius) : Renderable(pos), radius(radius) {}

    /**
     * Try to intersect the ray with the shape, and return only the closest t solution.
     * Ignore -t solutions.
     * Returns INFINITY otherwise.
     *
     * @param ray
     * @return t
     */
    double intersect(Ray ray) override {
        vec3 diff = ray.origin - position;

        double b = 2*(dot(diff, normalize(ray.direction) ));
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
            result = glm::min(t0,t1);
        }

        return result;
    }

    vec3 getNormalAt(vec3 point) override {
        return normalize(point - position);
    }
};


/**
 * Infinite plane
 */
class Plane : public Renderable {
public:
    vec3 normal;

    Plane() {}

    /**
     * Try to intersect the ray with the shape, and return only the closest t solution.
     * Ignore -t solutions.
     * Returns INFINITY otherwise.
     *
     * @param ray
     * @return t
     */
    double intersect(Ray ray) override {
        float denominator = dot(normal, normalize(ray.direction) );

        if (abs(denominator) > 0.000001f){
            float t = dot( (position - ray.origin), normal) / denominator;
            if (t>0)
                return t;
        }

        return INFINITY;
    }

    vec3 getNormalAt(vec3 point) override {
        return normalize(normal);
    }
};


/**
 * Class that represent every triangle in a mesh
 */
class Triangle : public Plane {

};

/**
 * Holding mesh informations
 */
struct Mesh {
    Material material;
    vector<Triangle *> triangles;
};


/**
 * Scene containing all objects
 */
struct Scene {
    Camera cam = Camera(vec3());
    vector<Light *> lights;
    vector<Renderable *> objs;
};




#endif //RAYTRACER_HELPERS_H
