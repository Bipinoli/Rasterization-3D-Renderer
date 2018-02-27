#pragma once

#include <cmath>

#include "vectormath.h"


struct Camera 
{
    float fov;
    int width, height;
    float w, h; // half of width and height in near plane
    float near;

    Camera(int width, int height, float fov = M_PI / 3, float near = 1) {
        // 60 deg field of vision
        this->width = width; this->height = height;
        this->fov = fov; this->near = near;

        h = tan(fov/2);
        w = h * (width / height);
    }

    Point rastarize(const Point& P) const;
};





Point Camera::rastarize(const Point& P) const {

    // project on to the near plane of camera

    // Camera eye is located at origin viewing in -ve z direction
    // so inorder to use z as a distance in depth buffer algorithm
    // z is made +ve
    Point prjPoint;
    if (P.z >= 0) {
        // not visible by camera
        prjPoint.z = -1; 
        return prjPoint;
        // rastrize returns rastarized 2D point so 
        // on success prjPoint.z = 0
        // so prjPoint.z can be used to identify failure
    }

    float z = - P.z; 

    
    prjPoint.x = near * P.x / z;
    prjPoint.y = near * P.y / z;


    // convert into raster space
    prjPoint.x = width/2.0 + (prjPoint.x / w) * width/2.0;
    prjPoint.y = height/2.0 + (prjPoint.y / h) * height/2.0;

    return prjPoint;
}