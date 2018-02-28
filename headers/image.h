#pragma once

#include <string>

#include "color.h"

class Image
{
protected:
	int width, height;
	Color* data;
	float* depthBuffer; // for Z Buffer algorithm
	float* shadowBuffer; // for Z Buffer from light's prespective

public:
	static constexpr float INF = 1e30;

	Image(const int width, const int height);

	virtual ~Image();
	
	int getWidth() const;
	int getHeight() const;

	Color getPixel(int x, int y) const;
	float getZDepth(int x, int y) const;
	float getShadowDepth(int x, int y) const;
	void setPixel(int x, int y, const Color c);
	void setZDepth(int x, int y, const float z);
	void setShadowDepth(int x, int y, const float z);



	void saveImagePPM(std::string filename) const;

};
