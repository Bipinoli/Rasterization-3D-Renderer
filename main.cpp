#include <iostream>
#include <algorithm>

#include "headers/vectormath.h"
#include "headers/homogenVector.h"
#include "headers/color.h"
#include "headers/image.h"
#include "headers/scene.h"
#include "headers/objParser.h"
#include "headers/camera.h"
#include "headers/zBuffer.h"
#include "headers/lightSrc.h"
#include "headers/transform.h"
#include "headers/matrix.h"

using namespace std;



int main(int argc, char** argv) {

    const int width = 1080;
    const int height = 720;

    const Point cameraPosition(2500, 1000,-5000);

    Image image(width, height);
    Camera camera(cameraPosition, width, height);
    LightSrc light(Point(-100, 100, -900), 1400.0f);
    Scene scene;


    ObjParser objParser;
    objParser.parse("box.obj.clean");


    for(auto t: objParser.triangles) {
        // cout << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << endl;
        // cout << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << endl;
        // cout << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << endl;



        scene.addTriangle(t);
    }

    world2view(scene, camera, light);
    rotateCameraX(-10, scene, light);
    rotateCameraY(20, scene, light);

    for (auto t: scene.triangles) {
        // cout << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << endl;
        // cout << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << endl;
        // cout << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << endl;
        // cout << "normals: " << endl;
        // cout << t.n0.x << ", " << t.n0.y << ", " << t.n0.z << endl;
        // cout << t.n1.x << ", " << t.n1.y << ", " << t.n1.z << endl;
        // cout << t.n2.x << ", " << t.n2.y << ", " << t.n2.z << endl;

    }

    findShadow(light, camera, scene, image);
    zBuffer(light, camera, scene, image);


    image.saveImagePPM("rastarized.ppm");


    
}