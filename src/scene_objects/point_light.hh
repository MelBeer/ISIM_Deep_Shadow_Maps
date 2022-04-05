#pragma once

#include "light_source.hh"

class PointLight : public LightSource
{
public:
    Point3 center;
    double radius;

    PointLight(Point3 center, double intensity, double radius)
    {
        this->center = center;
        this->intensity = intensity;
        this->radius = radius;
    }

    Vector3 GetLightVector(Point3 point) override; 
};