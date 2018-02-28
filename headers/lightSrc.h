#pragma once

#include "vectormath.h"

struct LightSrc {

	float intensity;
	Point position;

	LightSrc() {}
	LightSrc(const Point& pos, const float intensity = 50.0f) {
		this->position = pos;
		this->intensity = intensity;
	}
};