//
// Created by melbeer on 01/04/2022.
//

#ifndef RAYTRACER_PRINTER_H
#define RAYTRACER_PRINTER_H

#include <ostream>
#include "geometry.h"

std::ostream &operator<<(std::ostream &os, aiVector3D vec);

#endif //RAYTRACER_PRINTER_H
