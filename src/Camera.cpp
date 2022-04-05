//
// Created by melbeer on 03/04/2022.
//

#include "Camera.h"

Camera::Camera(aiVector3t<double> center, aiVector3t<double> lookAt, aiVector3t<double> up, double fovAlpha, double fovBeta,
               double nearClipPlane, double farClipPlane)
{
    this->center = center;
    this->lookAt = lookAt;
    this->up = up.Normalize();
    this->fovAlpha = fovAlpha;
    this->fovBeta = fovBeta;
    this->nearClipPlane = nearClipPlane;
    this->farClipPlane = farClipPlane;

    forward = (lookAt - center).Normalize();

    // Position plan image
    aiVector3t<double> positionPlan = center + forward * nearClipPlane;

    // Compute size of imagePlan
    planeWidth =
            nearClipPlane * tan((fovAlpha / 2.0f) * M_PI / 180.0f) * 2;
    planeHeight =
            nearClipPlane * tan((fovBeta / 2.0f) * M_PI / 180.0f) * 2;

    // Cross product
    aiVector3t<double> left = forward ^ this->up;
    right = -left;

    originPixel = positionPlan + left * (planeWidth / 2) + up * (planeHeight / 2);
}

aiVector3t<double> Camera::GetPixelPos(int posH, int posW)
{
    aiVector3t<double> pixelPosition = originPixel
                           + -up * pixelHeight * (double)posH
                           + right * pixelWidth * (double)posW;
    //std::cout << (posH + pixelHeight / 2) << '|'<< (posW + pixelWidth / 2) << " Pixel height " << pixelHeight << " Pixel w " << pixelWidth << '\n';
    return pixelPosition;
}

void Camera::SetPixelSize(int height, int width)
{
    pixelHeight = planeHeight / height;
    pixelWidth = planeWidth / width;
    originPixel = originPixel + -up * (pixelHeight / 2) + right * (pixelWidth / 2);
}
