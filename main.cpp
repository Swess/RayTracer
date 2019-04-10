#include <fstream>
#include <vector>
#include <cmath>
#include <CImg.h>
#include "NeededMath.h"
#include "geometry.h"

using namespace std;
using namespace cimg_library;

// Signatures
void loadScene(ifstream &file, Scene &scene);

Vec3 readVec3(ifstream &file);


// Main
int main() {
    Scene scene;

    ifstream inFile;
    string filename;

    // Ask the user for a scene file filename to parse
    cout << "Please enter a filename of a scenefile to load:" << endl;
    cin >> filename;

    // Opening the file
    inFile.open(filename);
    if (!inFile) {
        cerr << "Unable to open file " << filename;
        exit(1);   // call system to stop
    }

    // Create scene
    loadScene(inFile, scene);
    inFile.close();
    cout << "Scene successfully loaded." << endl;


    // Set const for shooting ray
    const float tanHalf = tan(scene.cam.fov / 2);
    const int HEIGHT = tan(scene.cam.fov / 2) * 2 *
                       scene.cam.focalLength;   // Here FOV has been loaded and converted to radians already.
    const int WIDTH = scene.cam.aspectRatio * HEIGHT;


    // Creates an image with three channels and sets it to black
    CImg<float> image(WIDTH, HEIGHT, 1, 3, 0);

    // Shoot rays
    for (int i = -WIDTH / 2; i < WIDTH / 2; i++) {
        // Current pixel x, calculated from ray.x coordinate (i)
        int imgX = i + WIDTH/2;

        for (int j = HEIGHT / 2; j > -HEIGHT/2; j--) {
            // Current pixel y, calculated from ray.y coordinate (j)
            int imgY = -j + HEIGHT/2;

            // Create ray for current pixel
            Ray ray = Ray(scene.cam.position, Vec3(i, j, -scene.cam.focalLength).normalize());

            // Check intersection with each objs
            double closestScalar = INFINITY;
            Renderable *closestObj;
            for (int k = 0; k < scene.objs.size(); k++) {

                // Return only value > 0
                double t = scene.objs[k]->intersect(ray);

                // The closest intersection only
                if (t < closestScalar && t > 0) {
                    closestScalar = t;
                    closestObj = scene.objs[k];
                }
            }

            // Color pixel at calculated intersection
            if(closestScalar < INFINITY){
                // Compute intersection world coord
                Vec3 pointIntersect = scene.cam.position + (ray.direction*closestScalar);

                // Cast shadow rays
                for(int l=0; l<scene.lights.size(); l++){
                    Light *light = scene.lights[l];
                    Vec3 shadowDir = light->position - pointIntersect;
                    Ray shadowRay = Ray(pointIntersect, shadowDir);

                    // TODO: Detect intersection between pointIntersect and light
                }

                // Color
                image(imgX,imgY,0) = closestObj->material.diffuse.x * 255.0f;
                image(imgX,imgY,1) = closestObj->material.diffuse.y * 255.0f;
                image(imgX,imgY,2) = closestObj->material.diffuse.z * 255.0f;
            }


        }
    }

    // scenes/scene1.txt

    // Save img
    image.save("render.bmp");

    // Display img
    CImgDisplay main_disp(image,"Render");
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }

    // End process
    return 0;
}


/**
 * Parse scene file and create relative objects to build the scene
 * @param file
 */
void loadScene(ifstream &file, Scene &scene) {
    string token;

    // Until there is no more tokens
    while (file >> token) {

        if (token == "camera") {
            for (int i = 0; i < 4; i++) {
                file >> token;

                if (token == "pos:") {
                    scene.cam.position = readVec3(file);
                } else if (token == "fov:") {
                    file >> token;
                    scene.cam.fov = std::stod(token) * (M_PI / 180);
                } else if (token == "f:") {
                    file >> token;
                    scene.cam.focalLength = std::stof(token);
                } else if (token == "a:") {
                    file >> token;
                    scene.cam.aspectRatio = std::stof(token);
                }
            }


        } else if (token == "sphere") {
            Sphere *sphere = new Sphere(Vec3(), 0);
            Material mat;

            for (int i = 0; i < 6; i++) {
                file >> token;
                if (token == "pos:") {
                    sphere->position = readVec3(file);
                } else if (token == "rad:") {
                    file >> token;
                    sphere->radius = std::stod(token);
                } else if (token == "amb:") {
                    mat.ambient = readVec3(file);
                } else if (token == "dif:") {
                    mat.diffuse = readVec3(file);
                } else if (token == "spe:") {
                    mat.specular = readVec3(file);
                } else if (token == "shi:") {
                    file >> token;
                    mat.shininess = std::stof(token);
                }
            }
            sphere->material = mat;

            // Add to scene
            scene.objs.push_back(sphere);

        } else if (token == "plane") {
            Plane *plane = new Plane();
            Material mat;

            for (int i = 0; i < 6; i++) {
                file >> token;
                if (token == "pos:") {
                    plane->position = readVec3(file);
                } else if (token == "nor:") {
                    plane->normal = readVec3(file);
                } else if (token == "amb:") {
                    mat.ambient = readVec3(file);
                } else if (token == "dif:") {
                    mat.diffuse = readVec3(file);
                } else if (token == "spe:") {
                    mat.specular = readVec3(file);
                } else if (token == "shi:") {
                    file >> token;
                    mat.shininess = std::stof(token);
                }
            }
            plane->material = mat;

            // Add to scene
            scene.objs.push_back(plane);

        } else if (token == "light") {
            Light *light = new Light();

            for (int i = 0; i < 3; i++) {
                file >> token;
                if (token == "pos:") {
                    light->position = readVec3(file);
                } else if (token == "dif:") {
                    light->diffuseColor = readVec3(file);
                } else if (token == "spe:") {
                    light->specularColor = readVec3(file);
                }
            }

            // Add to scene
            scene.lights.push_back(light);
        }
    }
}

/**
 * Read the next 3 tokens, considered as numerical values, and return a Vec3 out of them
 * @param file
 * @return
 */
Vec3 readVec3(ifstream &file) {
    double x, y, z;
    file >> x;
    file >> y;
    file >> z;
    return {x, y, z};
}

