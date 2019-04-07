#include <iostream>
#include "NeededMath.h"

#ifndef RAYTRACER_HELPERS_H
#define RAYTRACER_HELPERS_H

/**
 * Any SceneEntity
 */
class SceneObj {
public:
    Vec3 position = Vec3(0, 0, 0);

    SceneObj(Vec3 pos) {
        position = pos;
    }

    void setPosition(Vec3 pos) {
        this->position = pos;
    }
};

/**
 * The camera
 */
class Camera : public SceneObj {
public:
    float fov;
    float focalLength;

    Camera(const Vec3 &pos, float fov, float fl) : SceneObj(pos) {
        this->fov = fov;
        this->focalLength = fl;
    }

};


#endif //RAYTRACER_HELPERS_H
