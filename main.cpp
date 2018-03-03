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
    
    // -------------------------------------------------------------------------------
    // command line arguments: 
    //      main camPosX camPosY camPosZ lightX lightY lightZ rotX rotY rotZ
    // Note:if not provided they take on the default values
    // -------------------------------------------------------------------------------


    const int width = 1080;
    const int height = 720;

    bool cameraPositionSet = false;

    Point cameraPosition(0, 0, 0);
    Point lightPosition(0, 0, 0);

    if (argc >= 5) {
        stringstream ss;
        ss << argv[2];
        ss >> cameraPosition.x;
        ss.clear();
        ss << argv[3];
        ss >> cameraPosition.y;
        ss.clear();
        ss << argv[4];
        ss >> cameraPosition.z;
        ss.clear();
        cameraPositionSet = true;

        if (argc >= 8) {
            stringstream ss;
            ss << argv[5];
            ss >> lightPosition.x;
            ss.clear();
            ss << argv[6];
            ss >> lightPosition.y;
            ss.clear();
            ss << argv[7];
            ss >> lightPosition.z;
            ss.clear();
        }
    }


    Image image(width, height);
    Camera camera(cameraPosition, width, height);
    LightSrc light(lightPosition, 1000.0f);
    Scene scene;


    ObjParser objParser;
    objParser.parse(argv[1]);


    // automatically adjust camera to look at the object
    // by taking average coordinates of the vertices as the camera position
    // but if camera position is given as command line args don't set it automatically
    Point towardsObject;

    for(auto t: objParser.triangles) {
        
        if (!cameraPositionSet) {
            towardsObject.x += (t.v0.x + t.v1.x + t.v2.x) / (3.0 * objParser.triangles.size());
            towardsObject.y += (t.v0.y + t.v1.y + t.v2.y) / (3.0 * objParser.triangles.size());
            towardsObject.z += (t.v0.z + t.v1.z + t.v2.z) / (3.0 * objParser.triangles.size());
        }

        scene.addTriangle(t);
    }

    if (!cameraPositionSet) {

        const float CAMERA_DIST = 500;

        towardsObject.x *= NORMALIZED_2_RASTER_EXPANSION_SCALE;
        towardsObject.y *= NORMALIZED_2_RASTER_EXPANSION_SCALE;
        towardsObject.z *= NORMALIZED_2_RASTER_EXPANSION_SCALE;

        cout << "\n--- Average coordinate of the vertices in scene ----\n";
        cout << "avg x: " << towardsObject.x << endl;
        cout << "avg y: " << towardsObject.y << endl;
        cout << "avg z: " << towardsObject.z << endl;

        towardsObject.z += CAMERA_DIST;
        camera.position = towardsObject;

        // set light position automatically as well
        light.position = towardsObject;
        light.position.x += 50;
        light.position.y += 50;
        light.position.z -= CAMERA_DIST;
        light.position.z += 100;
    }

    cout << "\n\n-------- before transform -------------" << endl;

    cout << "camera position: " << camera.position.x << ", " << camera.position.y
             << ", " << camera.position.z << endl << endl;
    cout << "light position: " << light.position.x << ", " << light.position.y
             << ", " << light.position.z << endl << endl;
 

    cout << "One of the face before:" << endl;
    cout << scene.triangles[0].v0.x << ", " << scene.triangles[0].v0.y << ", " << scene.triangles[0].v0.z << endl;
    cout << scene.triangles[0].v1.x << ", " << scene.triangles[0].v1.y << ", " << scene.triangles[0].v1.z << endl;
    cout << scene.triangles[0].v2.x << ", " << scene.triangles[0].v2.y << ", " << scene.triangles[0].v2.z << endl;
    cout << "normals: " << endl;
    cout << scene.triangles[0].n0.x << ", " << scene.triangles[0].n0.y << ", " << scene.triangles[0].n0.z << endl;
    cout << scene.triangles[0].n1.x << ", " << scene.triangles[0].n1.y << ", " << scene.triangles[0].n1.z << endl;
    cout << scene.triangles[0].n2.x << ", " << scene.triangles[0].n2.y << ", " << scene.triangles[0].n2.z << endl;


    world2view(scene, camera, light);

    if (argc >= 11) {
        // rotation along x y z is given as the command line args
        float rx, ry, rz;
        stringstream ss;
        ss << argv[8];
        ss >> rx;
        ss.clear();
        ss << argv[9];
        ss >> ry;
        ss.clear();
        ss << argv[10];
        ss >> rz;
        ss.clear();
        rotateCameraX(rx, scene, light);
        rotateCameraY(ry, scene, light);
        rotateCameraZ(rz, scene, light);
    }

    cout << "\n\n------ after transform -----------" << endl;

    cout << "camera position: " << camera.position.x << ", " << camera.position.y
             << ", " << camera.position.z << endl << endl;
    cout << "light position: " << light.position.x << ", " << light.position.y
             << ", " << light.position.z << endl << endl;

    cout << "One of the face after:" << endl;
    cout << scene.triangles[0].v0.x << ", " << scene.triangles[0].v0.y << ", " << scene.triangles[0].v0.z << endl;
    cout << scene.triangles[0].v1.x << ", " << scene.triangles[0].v1.y << ", " << scene.triangles[0].v1.z << endl;
    cout << scene.triangles[0].v2.x << ", " << scene.triangles[0].v2.y << ", " << scene.triangles[0].v2.z << endl;
    cout << "normals: " << endl;
    cout << scene.triangles[0].n0.x << ", " << scene.triangles[0].n0.y << ", " << scene.triangles[0].n0.z << endl;
    cout << scene.triangles[0].n1.x << ", " << scene.triangles[0].n1.y << ", " << scene.triangles[0].n1.z << endl;
    cout << scene.triangles[0].n2.x << ", " << scene.triangles[0].n2.y << ", " << scene.triangles[0].n2.z << endl;

    // for (auto t: scene.triangles) {

    //     cout << t.v0.x << ", " << t.v0.y << ", " << t.v0.z << endl;
    //     cout << t.v1.x << ", " << t.v1.y << ", " << t.v1.z << endl;
    //     cout << t.v2.x << ", " << t.v2.y << ", " << t.v2.z << endl;
    //     cout << "normals: " << endl;
    //     cout << t.n0.x << ", " << t.n0.y << ", " << t.n0.z << endl;
    //     cout << t.n1.x << ", " << t.n1.y << ", " << t.n1.z << endl;
    //     cout << t.n2.x << ", " << t.n2.y << ", " << t.n2.z << endl;

    // }

    // findShadow(light, camera, scene, image);
    zBuffer(light, camera, scene, image);


    cout << endl;
    image.saveImagePPM("rastarized.ppm");


    
}