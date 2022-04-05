//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>
#include "printer.h"

aiVector3t<double> planeIntersect(const aiFace &face, const aiMesh &mesh, const aiVector3t<double> &origin, const aiVector3t<double> &ray,
                                 const aiVector3t<double> &nullvalue);

#endif //RAYTRACER_GEOMETRY_H
