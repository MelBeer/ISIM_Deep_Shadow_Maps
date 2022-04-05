//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#include <vector>
#include <fstream>
#include <iostream>
#include "geometry.h"

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
