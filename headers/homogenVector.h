#pragma once

#include "vectormath.h"


struct HomogenVector
{
	double x, y, z, h;
	double coords[4];

	HomogenVector(const Vector& v) {
		coords[0] = v.x;
		coords[1] = v.y;
		coords[2] = v.z;
		coords[3] = 1;

		update();
	}

	virtual ~HomogenVector() {}

	HomogenVector& operator =(const HomogenVector& v) {

		coords[0] = v.x;
		coords[1] = v.y;
		coords[2] = v.z;
		coords[3] = v.h;

		update();

		return *this;
	}

	void update() {
		x = coords[0]; 
		y = coords[1];
		z = coords[2];
		h = coords[3];
	}

	Vector generateVector() {
		return Vector((float)x/h, (float)y/h, (float)z/h);
	}
};