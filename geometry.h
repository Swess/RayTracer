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

        // Check if it was a backface, if so, ignore
        if(result<INFINITY){
            // Normal at point
            vec3 np = getNormalAt(ray.origin + (float)result*ray.direction);
            if( dot(normalize(np),normalize(ray.direction)) > 0 )
                return INFINITY;
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
     * Ignore for backface
     *
     * Returns INFINITY otherwise.
     *
     * @param ray
     * @return t
     */
    double intersect(Ray ray) override {
        float denominator = dot(normal, normalize(-ray.direction) );

        if (denominator > 0.000001f){
            float t = dot( (ray.origin - position), normal) / denominator;
            if (t>0) return t;
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
    vec3 p1;
    vec3 p2;
    vec3 p3;
public:
    Triangle(vec3 p1, vec3 p2, vec3 p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;

        normal = normalize(cross(p2-p1, p3-p1));
        position = p1;
    }

    double intersect(Ray ray) override {
        // Intersection with the general forming plane
        // This enforce backface culling
        double t = Plane::intersect(ray);
        if(t == INFINITY)
            return t;

        ///// Barycentric method
        // Finding the point
        vec3 point = ray.origin + (float)t*ray.direction;

        vec3 edge0 = p2-p1;
        vec3 edge1 = p3-p2;
        vec3 edge2 = p1-p3;

        vec3 C0 = point - p1;
        vec3 C1 = point - p2;
        vec3 C2 = point - p3;

        // Negative bias for clean edges and slight permission
        float edgeBias = -0.000001f;

        // Check if inside the triangle
        if(
                dot(normal, cross(edge0, C0)) > edgeBias
                && dot(normal, cross(edge1, C1)) > edgeBias
                && dot(normal, cross(edge2, C2)) > edgeBias
                ){
            return t;
        }
        return INFINITY;
    }
};

/**
 * Holding mesh informations
 */
struct Mesh {
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
