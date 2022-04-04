//
// Created by melbeer on 01/04/2022.
//

#include "printer.h"

std::ostream &operator<<(std::ostream &os, aiVector3D vec) {
    return os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
}
