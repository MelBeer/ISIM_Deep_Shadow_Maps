#pragma once
#include "vector3.hh"
#include "point3.hh"

class LightSource
{
public:
    float intensity = 0;

    virtual Vector3 GetLightVector(Point3 point) = 0;

};