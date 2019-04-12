# C++ RayTracer

Basic RayTracer that renders some primitives and meshs.
The current list of primitives supported are:
- Infinite Planes
- Triangles
- Spheres

For this proof of concept application, the camera is always looking toward -z, with an up vector of (0,1,0).

## Parser
A basic parser is included to support scene files to be read and rendered.
You will be asked to type the file path of the scene file to use.
Can load only load .obj right now.

Mesh files are load automatically form the /scenes folder

## Examples
Here are example scene files with their renders.

---
![Example1](examples/scene3.bmp)
[SceneFile1](examples/scene3.txt)

![Example2](examples/scene5.bmp)
[SceneFile2](examples/scene5.txt)

![Example3](examples/scene6.bmp)
[SceneFile3](examples/scene6.txt)

---

## Dependencies
- [CImg](http://cimg.eu/)
- [GLM]([https://glm.g-truc.net/])

Using GLM for basic structs and geomatric manipulation.
CImg is used to render/save/display the final image.