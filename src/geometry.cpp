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

    double kEpsilon = 0.0000001f;
    aiVector3t<double> v0 = mesh.mVertices[face.mIndices[0]];
    aiVector3t<double> v1 = mesh.mVertices[face.mIndices[1]];
    aiVector3t<double> v2 = mesh.mVertices[face.mIndices[2]];

    const auto edge1 = v1 - v0;
    const auto edge2 = v2 - v0;

    const auto pvec = ray ^ edge2;
    const auto det = edge1 * pvec;

    if (det < kEpsilon)
        return nullvalue;

    const auto tvec = origin - v0;

    const auto u = tvec * pvec;
    if (u < 0 || u > det)
        return nullvalue;
    

    const auto qvec = tvec ^ edge1;

    const auto v = ray * qvec;
    if (v < 0 || u + v > det)
        return nullvalue;

    const auto t = edge2 * qvec;

    const auto inv_det = 1 / det;

    return aiVector3t<double>(t * inv_det, u * inv_det, v * inv_det);
}
