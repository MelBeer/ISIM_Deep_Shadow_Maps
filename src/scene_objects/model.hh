#pragma once
#include <memory>
#include "texture_material.hh"
#include "vector3.hh"

class Model
{
protected:
    std::shared_ptr<TextureMaterial> texture;

public:
    virtual std::shared_ptr<Point3> RayIntersect(Point3 origin, Vector3 ray) = 0;
    virtual Vector3 GetNormal(Point3 point) = 0;
    virtual TextureInfos GetInfos(Point3 point) = 0;
};