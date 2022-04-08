//
// Created by melbeer on 05/04/2022.
//

#include "PointLight.h"

PointLight::PointLight(aiVector3t<double> position, double intensity) {
    this->position = position;
    this->intensity = intensity;
}
