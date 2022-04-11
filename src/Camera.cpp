//
// Created by melbeer on 03/04/2022.
//

#include "Camera.h"

Camera::Camera(aiVector3t<double> center, aiVector3t<double> lookAt, double fovAlpha, double aspectRatio,
               double nearClipPlane, double farClipPlane)
{
    this->center = center;
    this->lookAt = lookAt;
    this->fovAlpha = fovAlpha;
    this->nearClipPlane = nearClipPlane;
    this->farClipPlane = farClipPlane;

    forward = (lookAt - center).Normalize();

    // Position plan image
    aiVector3t<double> positionPlan = center + forward * nearClipPlane;

    // Compute size of imagePlan
    planeWidth =
            nearClipPlane * tan((fovAlpha / 2.0f) * M_PI / 180.0f) * 2;
    planeHeight = planeWidth * (1/aspectRatio);

    // Cross product
    right = (forward ^ aiVector3t<double>(0,0,1)).Normalize();

    up = (right ^ forward).Normalize();

    originPixel = positionPlan - right * (planeWidth / 2) + up * (planeHeight / 2);
}

aiVector3t<double> Camera::GetPixelPos(int posH, int posW)
{
    aiVector3t<double> pixelPosition = originPixel
                           + -up * pixelHeight * (double)posH
                           + right * pixelWidth * (double)posW;
    //std::cout << (posH + pixelHeight / 2) << '|'<< (posW + pixelWidth / 2) << " Pixel height " << pixelHeight << " Pixel w " << pixelWidth << '\n';
    return pixelPosition;
}

void Camera::SetPixelSize(unsigned int height, unsigned int width)
{
    pixelHeight = planeHeight / (double)height;
    pixelWidth = planeWidth / (double)width;
    originPixel = originPixel + -up * (pixelHeight / 2) + right * (pixelWidth / 2);
}
