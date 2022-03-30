//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>
#include "Image.h"

class Renderer {
private:
    const aiScene *scene;
    aiCamera *camera;
    aiVector3t<float> camRight;
    aiVector3t<float> originPixel;
    float pixelSize;
    float clipPlaneWidth;
    float clipPlaneHeight;
    float ratio;

public:
    Renderer(const aiScene *scene) {
        this->scene = scene;
        ratio = 16.0 / 9.0;
        setCamera(0);
    }
    void setCamera(uint nCamera);
    Image renderScene(int imgWidth);

private:
    aiVector3t<float>
    findClosestIntersectPt(aiVector3t<float> ray, aiFace &face, aiMesh &mesh, aiVector3t<float> nullvalue);
};


#endif //RAYTRACER_RENDERER_H
