#pragma once

#include <cmath>

#include "matrix.h"
#include "homogenVector.h"


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