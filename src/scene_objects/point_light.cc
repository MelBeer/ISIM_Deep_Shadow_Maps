#include "point_light.hh"

Vector3 PointLight::GetLightVector(Point3 point) {
    return center - point;
}