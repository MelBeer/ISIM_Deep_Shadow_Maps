#pragma once
#include <utility>

#include "model.hh"
#include "color.hh"
#include "uniform_texture.hh"

class Sphere : public Model
{
public:
    float radius;
    Point3 center;

    Sphere(Point3 center, float radius)
    {
        this->center = center;
        this->radius = radius;
        this->texture = std::make_shared<UniformTexture>();
    }

    Sphere(Point3 center, float radius, std::shared_ptr<TextureMaterial> texture)
    {
        this->center = center;
        this->radius = radius;
        this->texture = std::move(texture);
    }

    std::shared_ptr<Point3> RayIntersect(Point3 origin, Vector3 ray) override;
    Vector3 GetNormal(Point3 point) override;
    TextureInfos GetInfos(Point3 point) ;
};