#include <iostream>

#include "headers/vectormath.h"
#include "headers/color.h"
#include "headers/image.h"


struct Triangle 
{
    Point v0, v1, v2;
    Vector normal;

    Triangle(const Point& v0, const Point& v1, const Point& v2) {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;

        normal = cross(v2-v1, v0-v1);
    }
};



bool insideTriangle(const Point& Q, const Triangle& triangle) {

    const Point& v0 = triangle.v0;
    const Point& v1 = triangle.v1;
    const Point& v2 = triangle.v2;
    
    // following counter clockwise winding
    if ( dot( cross(v2-v1, Q-v1), triangle.normal) >= 0 &&
         dot( cross(v0-v2, Q-v2), triangle.normal) >= 0 &&
         dot( cross(v1-v0, Q-v0), triangle.normal) >= 0 )
    {
        return true;
    }
    return false;
}


Point baryCentricCoordinate(const Point& Q, const Triangle& triangle) {

    float area = cross(triangle.v2-triangle.v1, triangle.v0-triangle.v1).length();
    float l0Area = cross(triangle.v2-triangle.v1, Q-triangle.v1).length();
    float l1Area = cross(triangle.v0-triangle.v2, Q-triangle.v2).length();
    float l2Area = cross(triangle.v1-triangle.v0, Q-triangle.v0).length();

    return Point({ l0Area/area, l1Area/area, l2Area/area });

}




int main(int argc, char** argv) {

    const int width = 1080;
    const int height = 720;


    Point v0 = {491.407, 411.407, 0}; 
    Point v1 = {148.593, 68.5928, 0};
    Point v2 = {148.593, 411.407, 0}; 
    Color c0 = {1, 0, 0}; 
    Color c1 = {0, 1, 0}; 
    Color c2 = {0, 0, 1}; 

    Triangle triangle({v0, v1, v2});

    Image image(width, height);


    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {

            Point P({x+0.5f, y+0.5f, 0});
            if (insideTriangle(P, triangle)) {

                Point baryCentric = baryCentricCoordinate(P, triangle);
                Color* color = image.getPixel(x,y);
                *color = c0 * baryCentric.x + c1 * baryCentric.y + c2 * baryCentric.z;

            }
        }
    }

    image.saveImagePPM("rastarized.ppm");

}