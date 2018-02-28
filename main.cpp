#include <iostream>
#include <algorithm>

#include "headers/vectormath.h"
#include "headers/color.h"
#include "headers/image.h"
#include "headers/scene.h"
#include "headers/objParser.h"
#include "headers/camera.h"
#include "headers/zBuffer.h"
#include "headers/lightSrc.h"

using namespace std;





int main(int argc, char** argv) {

    const int width = 1080;
    const int height = 720;

    Image image(width, height);
    Camera camera(width, height);
    LightSrc light(Point(200, 200, -900), 1200.0f);
    Scene scene;


    // ObjParser objParser;
    // objParser.simpleParse("pumpkin.obj");

    // for(auto t: objParser.triangles) {
    //     // cout << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << endl;
    //     // cout << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << endl;
    //     // cout << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << endl;

    //     // t.c0 = c0;
    //     // t.c1 = c1;
    //     // t.c2 = c2;
    //     scene.addTriangle(t);
    // }

    Point p1({100, -100, -1000});
    Point p2({-100, 100, -900});
    Point p3({300, 100, -1000});
    Point p4({-100, -100, -1050});
    Point p5({-50, -50, -900});
    Point p6({250, 250, -1500});

    Color r({1,0,0});
    Color g({0,1,0});
    Color b({0,0,1});

    Vector n1({-50, -10, 100});
    Vector n2({50, 10, 100});
    Vector n3({10, -10, 50});


    Triangle t1({p1,p2,p3, r,r,r, n1,n2,n3});
    Triangle t2({p1,p2,p4, g,g,g, n2,n3,n3});
    Triangle t3({p5,p6,p2, b,b,b, n1+2*n2,n2+n1,n2});

    scene.addTriangle(t1);
    scene.addTriangle(t2);
    scene.addTriangle(t3);


    zBuffer(light, camera, scene, image);


    image.saveImagePPM("rastarized.ppm");


    
}