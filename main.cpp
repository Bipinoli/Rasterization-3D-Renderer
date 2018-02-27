#include <iostream>
#include <algorithm>

#include "headers/vectormath.h"
#include "headers/color.h"
#include "headers/image.h"
#include "headers/scene.h"
#include "headers/objParser.h"
#include "headers/camera.h"
#include "headers/zBuffer.h"

using namespace std;





int main(int argc, char** argv) {

    const int width = 1080;
    const int height = 720;

    Image image(width, height);
    Camera camera(width, height);
    Scene scene;


    ObjParser objParser;
    objParser.simpleParse("pumpkin.obj");

    for(auto t: objParser.triangles) {
        // cout << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << endl;
        // cout << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << endl;
        // cout << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << endl;

        // t.c0 = c0;
        // t.c1 = c1;
        // t.c2 = c2;
        scene.addTriangle(t);
    }


    zBuffer(camera, scene, image);


    image.saveImagePPM("rastarized.ppm");


    
}