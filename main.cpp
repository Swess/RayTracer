#include <fstream>
#include <vector>
#include "CImg.h"
#include "NeededMath.h"
#include "Helpers.h"

using namespace std;

// Signatures
void loadScene(ifstream &file, Scene &scene);

Vec3 readVec3(ifstream &file);


// Main
int main() {
    const int WIDTH = 1080;
    const int HEIGHT = 920;
    Scene scene;

    ifstream inFile;
    string filename;

    // Ask the user for a scene file filename to parse
    cout << "Please enter a filename of a scenefile to load:" << endl;
    cin >> filename;

    // Opening the file
    inFile.open(filename);
    if (!inFile) {
        cerr << "Unable to open file " << filename <<  filename;
        exit(1);   // call system to stop
    }

    cout << "Passed condition." << endl;

    // Create scene
    loadScene(inFile, scene);
    inFile.close();

    cout << "Scene successfully loaded." << endl;

    cout << scene.objs.size() << endl;


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
                    scene.cam.fov = std::stof(token);
                } else if (token == "f:") {
                    file >> token;
                    scene.cam.focalLength = std::stof(token);
                } else if (token == "a:") {
                    file >> token;
                    scene.cam.aspectRatio = std::stof(token);
                }
            }


        } else if (token == "sphere") {
            Sphere sphere = Sphere(Vec3(), 0);
            Material mat;

            for (int i = 0; i < 6; i++) {
                file >> token;
                if (token == "pos:") {
                    sphere.position = readVec3(file);
                } else if (token == "rad:") {
                    file >> token;
                    sphere.radius = std::stod(token);
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
            sphere.material = mat;

            // Add to scene
            scene.objs.push_back(sphere);

        } else if (token == "plane") {
            Plane plane = Plane();
            Material mat;

            for (int i = 0; i < 6; i++) {
                file >> token;
                if (token == "pos:") {
                    plane.position = readVec3(file);
                } else if (token == "nor:") {
                    plane.normal = readVec3(file);
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
            plane.material = mat;

            // Add to scene
            scene.objs.push_back(plane);

        } else if (token == "light") {
            Light light = Light();

            for (int i = 0; i < 3; i++) {
                file >> token;
                if (token == "pos:") {
                    light.position = readVec3(file);
                } else if (token == "dif:") {
                    light.diffuseColor = readVec3(file);
                } else if (token == "spe:") {
                    light.specularColor = readVec3(file);
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

