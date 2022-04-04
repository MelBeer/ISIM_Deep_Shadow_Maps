//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include "Camera.h"
#include "Image.h"

class Renderer {
private:
    const aiScene *scene;
    Camera camera;
    float pixelSize;
    float ratio;

public:
    Renderer(const aiScene *scene) {
        this->scene = scene;
        ratio = 16.0 / 9.0;
        setCamera();
    }
    void setCamera();
    Image renderScene(int imgWidth);

private:
    aiVector3t<float>
    findClosestIntersectPt(aiVector3t<float> ray, aiFace &face, aiMesh &mesh, aiVector3t<float> nullvalue);
};


#endif //RAYTRACER_RENDERER_H
