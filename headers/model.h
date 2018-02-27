#ifndef MODEL_H
#define MODEL_H

#include "vectormath.h"
#include <vector>
#include <iostream>

struct Face{
	std::vector<int>v,vn;
	Face():v(3),vn(3){}

};

class Model
{
    public:

        bool loadModel(const char *);

        std::vector<Vector> v;
        std::vector<Vector> vn;
        //vector<Vector> vt;
        std::vector<Face> f;


};

#endif // MODEL_H
