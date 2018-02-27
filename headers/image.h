#pragma once

#include <string>

#include "color.h"

class Image
{
protected:
	int width, height;
	Color* data;
	float* depthBuffer; // for Z Buffer algorithm

public:
	Image(const int width, const int height);

	virtual ~Image();
	
	int getWidth() const;
	int getHeight() const;

	Color getPixel(int x, int y) const;
	float getZDepth(int x, int y) const;
	void setPixel(int x, int y, const Color c);
	void setZDepth(int x, int y, const float z);


	void saveImagePPM(std::string filename) const;

};
