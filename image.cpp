#include <algorithm>
#include <iostream>
#include <fstream>

#include "headers/image.h"

Image::Image(const int width, const int height)
	: width(width), height(height)
{
	data = new Color[width * height];
	depthBuffer = new float[width * height];
	shadowBuffer = new float[width * height];
	for (int i=0; i<width*height; i++) {
		depthBuffer[i] = Image::INF;
		shadowBuffer[i] = Image::INF;
	}
}

Image::~Image()
{
	delete[] data;
	delete[] depthBuffer;
}

int Image::getWidth() const
{
	return width;
}

int Image::getHeight() const
{
	return height;
}

Color Image::getPixel(int x, int y) const
{
	return *(data + (x + y * width));
}

void Image::setPixel(int x, int y, const Color c)
{
	*(data + (x + y * width)) = c;
}

float Image::getZDepth(int x, int y) const
{
	return *(depthBuffer + (x + y * width));
}

float Image::getShadowDepth(int x, int y) const
{
	return *(shadowBuffer + (x + y * width));
}

void Image::setZDepth(int x, int y, const float z) {
	*(depthBuffer + (x + y * width)) = z;
}

void Image::setShadowDepth(int x, int y, const float z) {
	*(shadowBuffer + (x + y * width)) = z;
}

void Image::saveImagePPM(std::string filename) const {

	// data[] contains the RGB information with 0 = dark, 1 = full bright
	// and the float betn 0 to 1 representing in between color

	try {
		std::ofstream ofs(filename, std::ios::binary | std::ios::out);

		ofs << "P6\n" << width << " " << height << " 255" << std::endl;
		for (int i=height-1; i>=0; i--) {
			for (int j=0; j<width; j++) {
				ofs << (unsigned char)(std::max(0.0f, std::min(data[i*width + j].r, 1.0f)) * 255) 
					<< (unsigned char)(std::max(0.0f, std::min(data[i*width + j].g, 1.0f)) * 255)
					<< (unsigned char)(std::max(0.0f, std::min(data[i*width + j].b, 1.0f)) * 255);
			}
		}

		ofs.close();

	} catch(...) {
		std::cerr << "Something really terrible happened so " + filename + " file couldn't be created!!" << std::endl;
	}
	std::cout << "Rendered image to a \"" + filename + "\" file. (SUCCESS)" << std::endl;
}