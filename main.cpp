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


void translatePoint(Point& p, const Point& newOrigin) {

    HomogenVector p2(p);
    double tx = (double)newOrigin.x, ty = (double)newOrigin.y, tz = (double)newOrigin.z;
    Matrix scaleMat = translate(tx, ty, tz);
    p2 = transformVector(scaleMat, p2);
    p = p2.generateVector();
}

void scalePoint(Point& p, const float x) {

    HomogenVector p2(p);
    double x2 = x;
    Matrix scaleMat = scale(x2);
    p2 = transformVector(scaleMat, p2);
    p = p2.generateVector();
}

void rotatePointX(Point& p, const float theta) {

    HomogenVector p2(p);
    double theta2 = theta;
    Matrix scaleMat = rotateX(theta2);
    p2 = transformVector(scaleMat, p2);
    p = p2.generateVector();
}

void rotatePointY(Point& p, const float theta) {

    HomogenVector p2(p);
    double theta2 = theta;
    Matrix scaleMat = rotateY(theta2);
    p2 = transformVector(scaleMat, p2);
    p = p2.generateVector();
}

void rotatePointZ(Point& p, const float theta) {

    HomogenVector p2(p);
    double theta2 = theta;
    Matrix scaleMat = rotateZ(theta2);
    p2 = transformVector(scaleMat, p2);
    p = p2.generateVector();
}


void rotateCameraX(const float theta, Scene& scene, LightSrc& light) {
    rotatePointX(light.position, theta);
    for (auto& t: scene.triangles) {
        rotatePointX(t.v0, theta);
        rotatePointX(t.v1, theta);
        rotatePointX(t.v2, theta);
        rotatePointX(t.n0, theta);
        rotatePointX(t.n1, theta);
        rotatePointX(t.n2, theta);
    }
}

void rotateCameraY(const float theta, Scene& scene, LightSrc& light) {
    rotatePointY(light.position, theta);
    for (auto& t: scene.triangles) {
        rotatePointY(t.v0, theta);
        rotatePointY(t.v1, theta);
        rotatePointY(t.v2, theta);
        rotatePointY(t.n0, theta);
        rotatePointY(t.n1, theta);
        rotatePointY(t.n2, theta);
    }
}

void rotateCameraZ(const float theta, Scene& scene, LightSrc& light) {
    rotatePointZ(light.position, theta);
    for (auto& t: scene.triangles) {
        rotatePointZ(t.v0, theta);
        rotatePointZ(t.v1, theta);
        rotatePointZ(t.v2, theta);
        rotatePointZ(t.n0, theta);
        rotatePointZ(t.n1, theta);
        rotatePointZ(t.n2, theta);
    }
}

void world2view(Scene& scene, const Camera& camera, LightSrc& light) {

    translatePoint(light.position, camera.position);

    const float sc = 1000;

    for (auto& t: scene.triangles) {
        scalePoint(t.v0, sc);
        scalePoint(t.v1, sc);
        scalePoint(t.v2, sc);
        scalePoint(t.n0, sc);
        scalePoint(t.n1, sc);
        scalePoint(t.n2, sc);
        translatePoint(t.v0, camera.position);
        translatePoint(t.v1, camera.position);
        translatePoint(t.v2, camera.position);
        translatePoint(t.n0, camera.position);
        translatePoint(t.n1, camera.position);
        translatePoint(t.n2, camera.position);
        translatePoint(t.normal, camera.position);
    }
}



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


    zBuffer(light, camera, scene, image);


    image.saveImagePPM("rastarized.ppm");


    
}