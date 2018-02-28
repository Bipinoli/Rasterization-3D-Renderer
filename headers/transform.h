#pragma once

#include <cmath>

#include "matrix.h"
#include "homogenVector.h"
#include "scene.h"
#include "lightSrc.h"
#include "camera.h"

#define PI M_PI


HomogenVector& transformVector(Matrix& m, HomogenVector& v) {

	// matrix must be 4 by 4 matrix
	float vals[4] = {0,0,0,0};

	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			vals[i] += m(i,j)*v.coords[j];
		}
	}

	for (int i=0; i<4; i++)
		v.coords[i] = vals[i];

	v.update();

	return v;
}	


Matrix rotateZ(double theta){
	theta = theta*PI/180;
	Matrix m = Matrix::createIdentity(4);
	m(0,0) = cos(theta);
	m(0,1) = -sin(theta);
	m(1,0) = sin(theta);
	m(1,1) = cos(theta);
	return m;
}

Matrix rotateX(double theta){
	theta = theta*PI/180;
	Matrix m = Matrix::createIdentity(4);
	m(1,1) = cos(theta);
	m(1,2) = -sin(theta);
	m(2,1) = sin(theta);
	m(2,2) = cos(theta);
	return m;
}

Matrix rotateY(double theta){
	theta = theta*PI/180;
	Matrix m = Matrix::createIdentity(4);
	m(2,2) = cos(theta);
	m(2,0) = -sin(theta);
	m(0,2) = sin(theta);
	m(0,0) = cos(theta);
	return m;
}

Matrix scale(double factor){
	Matrix m = Matrix::createIdentity(4);
	m(0,0) = factor;
	m(1,1) = factor;
	m(2,2) = factor;
	return m;
}

Matrix translate(double tx, double ty, double tz){
	Matrix m = Matrix::createIdentity(4);
	m(0,3)=tx;
	m(1,3)=ty;
	m(2,3)=tz;
	return m;
}





// -------------------------------------------------------------------------

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

void sceneTransform(Scene& scene, const Point& point) {

	for (auto& t: scene.triangles) {
        translatePoint(t.v0, point);
        translatePoint(t.v1, point);
        translatePoint(t.v2, point);
        translatePoint(t.n0, point);
        translatePoint(t.n1, point);
        translatePoint(t.n2, point);
        translatePoint(t.normal, point);
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
    }

    sceneTransform(scene, camera.position);
}