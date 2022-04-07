//
// Created by melbeer on 05/04/2022.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

#include "geometry.h"

class PointLight {
public:
    aiVector3t<double> position;
    double intensity;

    PointLight() {}

    PointLight(aiVector3t<double> position, double intensity);
};


#endif //RAYTRACER_POINTLIGHT_H
