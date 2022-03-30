//
// Created by melbeer on 29/03/2022.
//

#include "geometry.h"
#include <iostream>

#include <cmath>

// TODO : Fix computation.
//  Right now : doesn't compute the exact point, works only for triangles, and isn't optimal anyway
aiVector3t<float> planeIntersect(aiFace *face, aiMesh *mesh, aiVector3t<float> *origin, aiVector3t<float> *ray,
                                 aiVector3t<float> nullvalue) {
    if (face->mNumIndices > 3)
        return nullvalue;

    float kEpsilon = 0.0001f;
    aiVector3t<float> a = mesh->mVertices[face->mIndices[0]];
    aiVector3t<float> b = mesh->mVertices[face->mIndices[1]];
    aiVector3t<float> c = mesh->mVertices[face->mIndices[2]];
    // compute plane's normal
    aiVector3t<float> ab = b - a;
    aiVector3t<float> ac = c - a;
    // no need to normalize
    aiVector3t<float> N = ab ^ ac; // N
    float area2 = N.Length();

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = N * *ray;
    if (std::fabs(NdotRayDirection) < kEpsilon) // almost 0
        return nullvalue; // they are parallel so they don't intersect !

    // compute d parameter using equation 2
    float d = -N * a;

    // compute t (equation 3)
    float t = -(N * *origin + d) / NdotRayDirection;

    // check if the triangle is in behind the ray
    if (t < 0) return nullvalue; // the triangle is behind

    // compute the intersection point using equation 1
    aiVector3t<float> P = *origin + t * *ray;

    // Step 2: inside-outside test
    aiVector3t<float> C; // vector perpendicular to triangle's plane 

    // edge 0
    aiVector3t<float> edge0 = b - a;
    aiVector3t<float> vp0 = P - a;
    C = edge0 ^ vp0;
    if (N * C < 0) return nullvalue; // P is on the right side

    // edge 1
    aiVector3t<float> edge1 = c - b;
    aiVector3t<float> vp1 = P - b;
    C = edge1 ^ vp1;
    if (N * C < 0) return nullvalue; // P is on the right side

    // edge 2
    aiVector3t<float> edge2 = a - c;
    aiVector3t<float> vp2 = P - c;
    C = edge2 ^ vp2;
    if (N * C < 0) return nullvalue; // P is on the right side;

    return b;
}
