#pragma once

#include "light_source.hh"

class PointLight : public LightSource
{
public:
    Point3 center;
    float radius;

    PointLight(Point3 center, float intensity, float radius)
    {
        this->center = center;
        this->intensity = intensity;
        this->radius = radius;
    }

    Vector3 GetLightVector(Point3 point) override; 
};