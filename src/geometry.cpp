//
// Created by melbeer on 29/03/2022.
//

#include "geometry.h"
#include <iostream>

#include <cmath>

// TODO : Fix computation.
//  Right now : doesn't compute the exact point, works only for triangles, and isn't optimal anyway
aiVector3t<double> planeIntersect(const aiFace &face, const aiMesh &mesh, const aiVector3t<double> &origin, const aiVector3t<double> &ray,
                                 const aiVector3t<double> &nullvalue) {
    if (face.mNumIndices > 3)
    {
        std::cout << "I'm not a triangle !\n";
        return nullvalue;
    }

    double kEpsilon = 0.0000001f;
    aiVector3t<double> v0 = mesh.mVertices[face.mIndices[0]];
    aiVector3t<double> v1 = mesh.mVertices[face.mIndices[1]];
    aiVector3t<double> v2 = mesh.mVertices[face.mIndices[2]];

    aiVector3t<double> a = v1 - v0;
    aiVector3t<double> b = v2 - v1;
    aiVector3t<double> c = v0 - v2;

    // compute the face's normal vector
    aiVector3t<double> n = a ^ b;

    // if the normal and the ray direction have the same sign, then the triangle
    // is facing the wrong way
    if (ray * n >= 0)
        return nullvalue;

    // compute the distance from the plane to (0, 0, 0)
    // (aka the fourth plane equation component)
    double D = -(n * v0);
    double t
            = -(n * origin + D) / (n * ray);
    if (t < 0)
        return nullvalue;

    // P = O + t * dir
    aiVector3t<double> P_off = ray * t;
    aiVector3t<double> P = origin + P_off;

    // check on which side of a, b, and c P is

    aiVector3t<double> v0_to_p = P - v0;
    aiVector3t<double> v0_cross = a ^ v0_to_p;
    if (v0_cross * n < -kEpsilon)
        return nullvalue;

    aiVector3t<double> v1_to_p = P - v1;
    aiVector3t<double> v1_cross = b ^ v1_to_p;
    if (v1_cross * n < -kEpsilon)
        return nullvalue;

    aiVector3t<double> v2_to_p = P - v2;
    aiVector3t<double> v2_cross = c ^ v2_to_p;
    if (v2_cross * n < -kEpsilon)
        return nullvalue;

    return P;
}
