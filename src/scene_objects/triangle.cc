#include "triangle.hh"
#include <cmath>
using namespace std;

Vector3 Triangle::GetNormal(Point3 point)
{
    Vector3 ab = b.pointToVector() - a.pointToVector();
    Vector3 ac = c.pointToVector() - a.pointToVector();
    Vector3 cross = ab.cross(ac);
    return cross / cross.norm();
}

std::shared_ptr<Point3> Triangle::RayIntersect(Point3 origin, Vector3 ray)
{
    double kEpsilon = 0.0001f;
       // compute plane's normal
    Vector3 ab = b - a; 
    Vector3 ac = c - a; 
    // no need to normalize
    Vector3 N = ab.cross(ac); // N 
    double area2 = N.magnitude(); 
 
    // Step 1: finding P
 
    // check if ray and plane are parallel ?
    double NdotRayDirection = N.dotProduct(ray); 
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
        return nullptr; // they are parallel so they don't intersect ! 
 
    // compute d parameter using equation 2
    double d = -N.dotProduct(a.pointToVector()); 
 
    // compute t (equation 3)
    double t = -(N.dotProduct(origin.pointToVector()) + d) / NdotRayDirection; 
 
    // check if the triangle is in behind the ray
    if (t < 0) return nullptr; // the triangle is behind 
 
    // compute the intersection point using equation 1
    Vector3 P = origin.pointToVector() + t * ray; 
 
    // Step 2: inside-outside test
    Vector3 C; // vector perpendicular to triangle's plane 
 
    // edge 0
    Vector3 edge0 = b - a; 
    Vector3 vp0 = P - a.pointToVector(); 
    C = edge0.cross(vp0); 
    if (N.dotProduct(C) < 0) return nullptr; // P is on the right side 
 
    // edge 1
    Vector3 edge1 = c - b; 
    Vector3 vp1 = P - b.pointToVector(); 
    C = edge1.cross(vp1); 
    if (N.dotProduct(C) < 0)  return nullptr; // P is on the right side 
 
    // edge 2
    Vector3 edge2 = a - c; 
    Vector3 vp2 = P - c.pointToVector(); 
    C = edge2.cross(vp2); 
    if (N.dotProduct(C) < 0) return nullptr; // P is on the right side; 
 
    return make_shared<Point3>(b);
}

TextureInfos Triangle::GetInfos(Point3 point)
{
    return texture->GetTextureInfos(point);
}