#pragma once


#include "vectormath.h"
#include "camera.h"
#include "scene.h"
#include "image.h"



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


void zBuffer(const Camera& camera, const Scene& scene, Image& image) {

    const int width = image.getWidth();
    const int height = image.getHeight();

    for (const auto& triangle: scene.triangles) {


        // project triangle vertices
        Point prjPnt0 = camera.rastarize(triangle.v0);
        Point prjPnt1 = camera.rastarize(triangle.v1);
        Point prjPnt2 = camera.rastarize(triangle.v2);
        Point verts[] = {prjPnt0, prjPnt1, prjPnt2};
        Triangle prjTriangle({prjPnt0, prjPnt1, prjPnt2});

        if (prjPnt0.z != 0 || prjPnt1.z != 0 || prjPnt2.z != 0) {
            // point is backside of the camera so is not visible
            // if any point is not visible lets discard the whole triangle
            // with a huge polymesh with lots of triangle, triangles are
            // so small that this wouldn't cause noticable problem
            // like when one point is not visible but others are 
            // so some part of triangle is visible
            continue;
        }


        // find enveloping 2D box around the triangle
        int envXmin, envXmax, envYmin, envYmax;
        const int INF = 1e9;
        envXmin = envYmin = INF; envXmax = envYmax = -INF;

        for (int i=0; i<3; i++) {
            envXmin = std::min(envXmin, (int)verts[i].x);
            envXmax = std::max(envXmax, (int)verts[i].x);
            envYmin = std::min(envYmin, (int)verts[i].y);
            envYmax = std::max(envYmax, (int)verts[i].y);
        }

        envXmin = std::max(0, std::min(envXmin, width-1));
        envXmax = std::max(0, std::min(envXmax, width-1));
        envYmin = std::max(0, std::min(envYmin, height-1));
        envYmax = std::max(0, std::min(envYmax, height-1));


        // cout << "envXmin: " << envXmin
        //      << "envXmax: " << envXmax
        //      << "envYmin: " << envYmin 
        //      << "envYmax: " << envYmax << endl;


        // work with pixels within the envelope
        for (int y = envYmin; y <= envYmax; y++) {
            for (int x = envXmin; x <= envXmax; x++) {

                Point P({x+0.5f, y+0.5f, 0});
                if (insideTriangle(P, prjTriangle)) {

                    Point baryCentric = baryCentricCoordinate(P, prjTriangle);

                    float zBaryCen = triangle.v0.z * baryCentric.x + 
                                     triangle.v1.z * baryCentric.y + 
                                     triangle.v2.z * baryCentric.z;


                    if (zBaryCen < image.getZDepth(x, y)) {
                        image.setZDepth(x, y, zBaryCen);

                        // looking at triangle normally there must be equal distribution
                        // of colors
                        // but when the triangle is skewed color distribution mustn't be uniform
                        // so color should be adjusted according to the depth

                        Color color = (triangle.c0*(1/triangle.v0.z) * baryCentric.x + 
                                      triangle.c1*(1/triangle.v1.z) * baryCentric.y + 
                                      triangle.c2*(1/triangle.v2.z) * baryCentric.z) * zBaryCen;
                        image.setPixel(x, y, color);


                        // cout << "depth[" << x << "," << y << "] : " << image.getZDepth(x,y);
                    }
                }
            }
        }


    }

}

