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

    Camera(const Vec3 &pos, float fov, float fl) : SceneObj(pos) {
        this->fov = fov;
        this->focalLength = fl;
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
};


/**
 * Infinite plane
 */
class Plane : public Renderable {
public:
    Vec3 normal;

    Plane() {}
};


/**
 * Scene containing all objects
 */
struct Scene {
    Camera cam = Camera(Vec3());
    vector<Light> lights;
    vector<Renderable> objs;
};


#endif //RAYTRACER_HELPERS_H
