#include <fstream>
#include <vector>
#include <cmath>
#include <CImg.h>
#include "glm.hpp"
#include "OBJloader.h"
#include "NeededMath.h"
#include "geometry.h"

using namespace std;
using namespace cimg_library;
using namespace glm;

// Signatures
void loadScene(ifstream &file, Scene &scene);

vec3 readVec3(ifstream &file);


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
        int imgX = i + WIDTH / 2;

        for (int j = HEIGHT / 2; j > -HEIGHT / 2; j--) {
            // Current pixel y, calculated from ray.y coordinate (j)
            int imgY = -j + HEIGHT / 2;
            double t;
            vec3 pixelColor = vec3();

            // Create ray for current pixel
            Ray ray = Ray(scene.cam.position, normalize(vec3(i, j, -scene.cam.focalLength)) );

            // Check intersection with each objs
            float closestScalar = INFINITY;
            Renderable *closestObj;
            for (int k = 0; k < scene.objs.size(); k++) {
                // Return only value > 0
                t = scene.objs[k]->intersect(ray);

                // The closest intersection only
                if (t < closestScalar && t > 0) {
                    closestScalar = t;
                    closestObj = scene.objs[k];
                }
            }

            // Color pixel at calculated intersection
            if (closestScalar < INFINITY) {
                // Compute intersection world coord
                vec3 pointIntersect = ray.origin + (ray.direction * closestScalar);
                vec3 result = vec3();    // Will contain the diffuse + specular contributions of the lights

                // Cast shadow rays
                float bias = 0.001f;
                for (int l = 0; l < scene.lights.size(); l++) {
                    Light *light = scene.lights[l];

                    vec3 normal = closestObj->getNormalAt(pointIntersect);
                    vec3 shadowDir = light->position - pointIntersect;
                    Ray shadowRay = Ray(pointIntersect + normal * bias, normalize(shadowDir) );

                    // Check if in shadow or not
                    bool lit = true;
                    for (int k = 0; k < scene.objs.size(); k++) {
                        t = scene.objs[k]->intersect(shadowRay);

                        // If we detect something in between intersection and light
                        if (t < INFINITY) {
                            lit = false;
                            break;
                        }
                    }

                    // If still considered in the light
                    if (lit) {
                        // Computing Phong Model
                        vec3 light_reflection = reflect(normalize(-shadowRay.direction), normalize(normal) );
                        vec3 diffuseCoef = closestObj->material.diffuse * (float)glm::max(dot(normalize(normal), normalize(shadowRay.direction) ), 0.0);
                        vec3 specularCoef = closestObj->material.specular * (float)pow(glm::max(dot(light_reflection, -ray.direction), 0.0), closestObj->material.shininess);

                        // Diffuse
                        result += light->diffuseColor * diffuseCoef;

                        // Specular
                        result += light->specularColor * specularCoef;
                    }
                }

                // Adding ambient + result
                pixelColor += closestObj->material.ambient + result;

                // Scale and clamp color
                pixelColor = pixelColor * 255.f;
                clampColor(pixelColor);

                // Paint the pixel
                image(imgX,imgY,0) = pixelColor.x;
                image(imgX,imgY,1) = pixelColor.y;
                image(imgX,imgY,2) = pixelColor.z;
            }


        }
    }

    // Save img
    image.save("render.bmp");

    // Display img
    CImgDisplay main_disp(image, "Render");
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
            Sphere *sphere = new Sphere(vec3(0,0,0), 0);
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
        } else if(token == "mesh"){
            Mesh mesh;
            Material mat;

            for(int i=0; i<5; i++){
                file >> token;

                if(token == "file:"){
                    file >> token;
                    // Load OBJ and create triangles for the mesh
                    string path = "scenes/";
                    path.append(token);

                    vector<vec3> vertices;
                    vector<vec3> normals;
                    vector<vec2> UVs;

                    // Load the OBJ data
                    loadOBJ(path.c_str(), vertices, normals, UVs);

                    // Build triangle out of the vertices data
                    for(int t=0; t<vertices.size(); t+=3){
                        Triangle *tri = new Triangle(vertices[t], vertices[t+1], vertices[t+2]);
                        mesh.triangles.push_back(tri);
                    }


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

            // Assign material to all triangles
            // TODO: structure this in a better way
            for(int k=0; k<mesh.triangles.size(); k++){
                mesh.triangles[k]->material = mat;
                scene.objs.push_back(mesh.triangles[k]);
            }
        }
    }
}

/**
 * Read the next 3 tokens, considered as numerical values, and return a Vec3 out of them
 * @param file
 * @return
 */
vec3 readVec3(ifstream &file) {
    double x, y, z;
    file >> x;
    file >> y;
    file >> z;
    return {x, y, z};
}

