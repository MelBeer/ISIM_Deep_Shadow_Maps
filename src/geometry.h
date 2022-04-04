//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_GEOMETRY_H
#define RAYTRACER_GEOMETRY_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>
#include "printer.h"

aiVector3t<float> planeIntersect(aiFace *face, aiMesh *mesh, aiVector3t<float> *origin, aiVector3t<float> *ray,
                                 aiVector3t<float> nullvalue);

#endif //RAYTRACER_GEOMETRY_H
