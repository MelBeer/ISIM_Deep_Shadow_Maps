#pragma once

#include "model.hh"
#include "color.hh"
#include "uniform_texture.hh"


class Triangle : public Model
{
public:
    Point3 a;
    Point3 b; 
    Point3 c; 

    Triangle(Point3 _a, Point3 _b, Point3 _c, std::shared_ptr<TextureMaterial> texture)
    {
        a = _a;
        b = _b; 
        c = _c; 
        this->texture = std::move(texture);
    }

    std::shared_ptr<Point3> RayIntersect(Point3 origin, Vector3 ray) override;
    Vector3 GetNormal(Point3 point) override;
    TextureInfos GetInfos(Point3 point) ;
};