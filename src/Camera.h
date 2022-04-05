//
// Created by melbeer on 03/04/2022.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "geometry.h"

class Camera {
public :
    aiVector3t<double> imagePlanPosition;
    aiVector3t<double> originPixel;
    double planeWidth;
    double planeHeight;
    double pixelHeight = 0;
    double pixelWidth = 0;
    aiVector3t<double> center;
    aiVector3t<double> lookAt;
    aiVector3t<double> up;
    aiVector3t<double> forward;
    aiVector3t<double> right;
    double fovAlpha;
    double fovBeta;
    double nearClipPlane;
    double farClipPlane;

    Camera() {

    };

    Camera(aiVector3t<double> center, aiVector3t<double> lookAt, double fovAlpha, double aspectRatio,
           double nearClipPlane, double farClipPlane);

    void SetPixelSize(int height, int width);

    aiVector3t<double> GetPixelPos(int posH, int posW);

};


#endif //RAYTRACER_CAMERA_H
