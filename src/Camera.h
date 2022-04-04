//
// Created by melbeer on 03/04/2022.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "geometry.h"

class Camera {
public :
    aiVector3t<float> imagePlanPosition;
    aiVector3t<float> originPixel;
    float planeWidth;
    float planeHeight;
    float pixelHeight = 0;
    float pixelWidth = 0;
    aiVector3t<float> center;
    aiVector3t<float> lookAt;
    aiVector3t<float> up;
    aiVector3t<float> forward;
    aiVector3t<float> right;
    float fovAlpha;
    float fovBeta;
    float nearClipPlane;
    float farClipPlane;

    Camera() {

    };

    Camera(aiVector3t<float> center, aiVector3t<float> lookAt, aiVector3t<float> up, float fovAlpha, float fovBeta,
           float nearClipPlane, float farClipPlane);

    void SetPixelSize(int height, int width);

    aiVector3t<float> GetPixelPos(int posH, int posW);

};


#endif //RAYTRACER_CAMERA_H
