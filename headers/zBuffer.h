#pragma once

#include <cmath>

#include "vectormath.h"
#include "camera.h"
#include "scene.h"
#include "image.h"
#include "lightSrc.h"
#include "transform.h"

#include <iostream>
using namespace std;


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


Vector reflect(const Vector& light, const Vector& normal) {

	// it needs to have a unit normal vector
	return light - 2 * dot(light, normal) * normal;
}


float findIllumination(const Point& point, const Vector& normal, const LightSrc& light,
					   const float ka, const float kd, const float ks, const float ns = 16)
{
	const Vector L = point - light.position;
	Vector N = normal; N.normalize();
	Vector R = reflect(L, N).normalized();
	Vector V = point; V.normalize();
	// because viewing from the origin


    float L_dot_N = -dot(L,N);
    float R_dot_V = -dot(R,V);
    if (L_dot_N < 0) L_dot_N = 0;
    if (R_dot_V < 0) R_dot_V = 0;

	// Ambient + Diffused + Specular
	float intensity = ka + 
					  kd * L_dot_N * light.intensity * (1 / L.length2()) +
					  ks * pow(R_dot_V, ns) * light.intensity * (1 / L.length2());


    // cout << "ka part: " << ka << endl;
    // cout << "kd part: " << kd * L_dot_N * light.intensity * (1 / L.length2()) << endl;
    // cout << "ks part: " << ks * pow(R_dot_V, ns) * light.intensity * (1 / L.length2()) << endl;

	return intensity;
}






void zBuffer(const LightSrc& light, Camera& camera, 
			 const Scene& scene, Image& image) 
{
	const float ka = 0.1, kd = 0.6, ks = 0.3, ns = 512; 

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


        // work with pixels within the envelope
        for (int y = envYmin; y <= envYmax; y++) {
            for (int x = envXmin; x <= envXmax; x++) {

                Point P({x+0.5f, y+0.5f, 0});
                if (insideTriangle(P, prjTriangle)) {

                    Point baryCentric = baryCentricCoordinate(P, prjTriangle);

                    // as zBaryCen is a distance it is made +ve
                    float zBaryCen = -triangle.v0.z * baryCentric.x + 
                                     -triangle.v1.z * baryCentric.y + 
                                     -triangle.v2.z * baryCentric.z;


                    if (zBaryCen < image.getZDepth(x, y)) {
                        image.setZDepth(x, y, zBaryCen);

                        // looking at triangle normally there must be equal distribution
                        // of colors
                        // but when the triangle is skewed color distribution mustn't be uniform
                        // so color should be adjusted according to the depth


                        Point hitPoint(x+0.5f, y+0.5f, -zBaryCen);


                        Color color = (triangle.c0*(1/triangle.v0.z) * baryCentric.x + 
                                      triangle.c1*(1/triangle.v1.z) * baryCentric.y + 
                                      triangle.c2*(1/triangle.v2.z) * baryCentric.z) * -zBaryCen;

                        // Phong model, finding Normal at a point of object by interpolation
                        Vector normal = (triangle.n0*(1/triangle.v0.z) * baryCentric.x + 
                                      	triangle.n1*(1/triangle.v1.z) * baryCentric.y + 
                                      	triangle.n2*(1/triangle.v2.z) * baryCentric.z) * -zBaryCen;


                        color *= findIllumination(hitPoint, normal, light,
                        						  ka, kd, ks, ns);

                        image.setPixel(x, y, color);

                        // ----------------------------------------------------------------------
                        // Identify if that pixel is in shadow

                        /*

                        // const float zShadow = (hitPoint - light.position).length();
                        const float zShadow = -(hitPoint.z - light.position.z);
                        // this demands that light source be placed facing front side
                        // as hitPoint.z should be more forward in the -ve z axis

                        // project hitPoint on the screen as viewed from the light source
                        // Since we have already populated shadowBuffer 
                        // so on the projected point if the shadowDepth is < zShadow 
                        // then there is other object more close to shadow than this
                        // so this pixel must be in shadow

                        // lets not bother about oerienting the camera at light source
                        // to view the scene properly
                        // warning: this may result in some part of scene not visible to 
                        //      the camera at light source
                        //   this can cause some wierd looking shadow 
                        //   but that can be avoided by readjusting the position of
                        //   light source so lets go with that


                        translatePoint(camera.position, light.position);
                        translatePoint(hitPoint, light.position);

                        Point shadowPixel = camera.rastarize(hitPoint);

                        if (shadowPixel.z == 0) {
                            // not backside from there

                            float zShadow2 = image.getShadowDepth(shadowPixel.x, shadowPixel.y);
                            if (zShadow2 < zShadow) {
                                // shadow 
                                cout << "shadow found" << endl;
                                image.setPixel(x, y, image.getPixel(x,y)*0.0);
                            }
                        }

                        translatePoint(camera.position, -light.position);

                        */

                    }
                }
            }
        }


    }

}

void findShadow(const LightSrc& light, Camera& camera, 
                Scene& scene, Image& image) 
{

    // view from the light source 
    // for which transform camera to the light source

    // lets not bother about oerienting the camera at light source
    // to view the scene properly
    // warning: this may result in some part of scene not visible to 
    //      the camera at light source
    //   this can cause some wierd looking shadow 
    //   but that can be avoided by readjusting the position of
    //   light source, so lets go with that
    translatePoint(camera.position, light.position);    
    sceneTransform(scene, light.position);


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


        // work with pixels within the envelope
        for (int y = envYmin; y <= envYmax; y++) {
            for (int x = envXmin; x <= envXmax; x++) {

                Point P({x+0.5f, y+0.5f, 0});
                if (insideTriangle(P, prjTriangle)) {

                    Point baryCentric = baryCentricCoordinate(P, prjTriangle);

                    // as zBaryCen is a distance it is made +ve
                    float zBaryCen = -triangle.v0.z * baryCentric.x + 
                                     -triangle.v1.z * baryCentric.y + 
                                     -triangle.v2.z * baryCentric.z;


                    if (zBaryCen < image.getShadowDepth(x, y)) {
                        image.setShadowDepth(x, y, zBaryCen);
                    }
                }
            }
        }

    }

    // transform camera to the original position
    translatePoint(camera.position, -light.position);    
    sceneTransform(scene, -light.position);
}
