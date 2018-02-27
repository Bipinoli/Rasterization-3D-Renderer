#include "headers/model.h"
#include <cstring>
#include <stdio.h>


bool Model::loadModel(const char* modelPath){


    FILE * file = fopen(modelPath, "r");
    if( file == NULL ){
        return false;
    }
    while(1){
        char lineHeader[1024];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v" ) == 0 ){
            Vector tempV;
            fscanf(file, "%lf %lf %lf\n", &tempV.x, &tempV.y, &tempV.z);
            v.push_back(tempV);

        }

        //'vn' is normal vector to the vertex and may not be unit vectors
        else if(strcmp(lineHeader, "vn")==0){
            Vector tempN;
            fscanf(file, "%lf %lf %lf\n", &tempN.x, &tempN.y, &tempN.z);
            vn.push_back(tempN);
        }
        //'f' for faces
        else if(strcmp(lineHeader,"f")==0){
            int v1I,v2I,v3I;
            int n1I, n2I,n3I;
            int temp;
            fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1I,&temp,&n1I,&v2I,&temp,&n2I,&v3I,&temp,&n3I);
            Face tempFace;
            tempFace.v[0] = v1I-1;
            tempFace.vn[0] = n1I-1;

            tempFace.v[1] = v2I-1;
            tempFace.vn[1] = n2I-1;

            tempFace.v[2] = v3I-1;
            tempFace.vn[2] = n3I-1;
            f.push_back(tempFace);

        }

    }
    return true;
}