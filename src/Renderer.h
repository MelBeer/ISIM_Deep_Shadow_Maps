//
// Created by melbeer on 29/03/2022.
//

#ifndef RAYTRACER_RENDERER_H
#define RAYTRACER_RENDERER_H

#include <thread>
#include <chrono>

#include "Camera.h"
#include "Image.h"

class Renderer {
private:
    const aiScene *scene;
    Camera camera;
    double pixelSize;
    double ratio;

public:
    Renderer(const aiScene *scene) {
        this->scene = scene;
        ratio = 16.0 / 9.0;
        setCamera();
    }
    void setCamera();
    Image renderScene(int imgWidth, int imgHeight);

private:

    aiVector3t<double>
            findClosestIntersectPt(aiVector3t<double> ray, int &face, int &mesh, aiVector3t<double> nullvalue);
    
    void ThreadFunction(const aiVector3t<double> &refPixel, double w, double h, Image &image);
};


#endif //RAYTRACER_RENDERER_H
