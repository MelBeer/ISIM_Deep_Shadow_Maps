//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>

#include <bitset>
#include <fstream>
#include <iostream>
using namespace std;

class Image {
public:
    int h;
    int w;
    std::vector<aiColor3D> pixels;

    Image(int h, int w, std::vector<aiColor3D> pixels = std::vector<aiColor3D>())
    {
        this->h = h;
        this->w = w;
        this->pixels = pixels;
    }

    void saveImage(const std::string& fileName);
};


#endif //RAYTRACER_IMAGE_H
