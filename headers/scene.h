#pragma once

#include <vector>

struct Triangle 
{
    Point v0, v1, v2;
    Color c0, c1, c2;
    Vector n0, n1, n2; // vertex normals
    Vector normal;



    Triangle(const Point& v0, const Point& v1, const Point& v2,
    		 const Color& c0 = Color(1.0f), 
    		 const Color& c1 = Color(1.0f),
    		 const Color& c2 = Color(1.0f),
             const Vector& n0 = Vector(0.0f),
             const Vector& n1 = Vector(0.0f),
             const Vector& n2 = Vector(0.0f))
	{
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
        this->c0 = c0;
        this->c1 = c1;
        this->c2 = c2;
        this->n0 = n0;
        this->n1 = n1;
        this->n2 = n2;

        normal = cross(v2-v1, v0-v1);
    }

};


struct Scene
{
	std::vector<Triangle> triangles;

	void addTriangle(const Triangle& triangle) {
		triangles.push_back(triangle);
	}
};