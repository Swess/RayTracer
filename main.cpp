#include <iostream>
#include "CImg.h"
#include "NeededMath.h"
#include "Helpers.h"



int main() {
    const int WIDTH = 1080;
    const int HEIGHT = 920;

    Vec3 test = Vec3(4,5,6);
    test.x = 2;

    std::cout << "Test.x : " << test.x << std::endl;

    return 0;
}