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


    const int height;
    const int width;    
    std::vector<aiVector3t<double>> pixels;

    Image(const int height, const int width, std::vector<aiVector3t<double>> pixels = std::vector<aiVector3t<double>>())
    : height(height), width(width), pixels(pixels)
    {}

    unsigned int getIndex(int h, int w)

    {
        return h * width + w;
    }

    void saveImage(const std::string& fileName);
};


#endif //RAYTRACER_IMAGE_H
