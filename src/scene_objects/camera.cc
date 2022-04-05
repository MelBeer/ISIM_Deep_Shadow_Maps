#include "camera.hh"

#include <cmath>

Camera::Camera(aiVector3t<double> center, aiVector3t<double> lookAt, aiVector3t<double> up, double fovAlpha, double fovBeta,
               double nearClipPlane, double farClipPlane)
{
    this->center = center;
    this->lookAt = lookAt;
    this->up = up.normalized();
    this->fovAlpha = fovAlpha;
    this->fovBeta = fovBeta;
    this->nearClipPlane = nearClipPlane;

    forward = (lookAt - center).normalized();

    // Position plan image
    Point3 positionPlan = center + forward * nearClipPlane;

    // Compute size of imagePlan
    planeWidth =
        nearClipPlane * tan((fovAlpha / 2.0f) * M_PI / 180.0f) * 2;
    planeHeight =
        nearClipPlane * tan((fovBeta / 2.0f) * M_PI / 180.0f) * 2;

    Vector3 down = -up;
    // Cross product
    Vector3 left = forward.cross(up).normalized();
    right = -left;

    originPixel = positionPlan + left * (planeWidth / 2) + up * (planeHeight / 2);
}

Point3 Camera::GetPixelPos(int posH, int posW)
{
    Point3 pixelPosition = originPixel
        + -up * pixelHeight * (posH)
        + right * pixelWidth * (posW);
    //std::cout << (posH + pixelHeight / 2) << '|'<< (posW + pixelWidth / 2) << " Pixel height " << pixelHeight << " Pixel w " << pixelWidth << '\n';
    return pixelPosition;
}

void Camera::SetPixelSize(int height, int width)
{
    pixelHeight = planeHeight / height;
    pixelWidth = planeWidth / width;
    originPixel = originPixel + -up * (pixelHeight / 2) + right * (pixelWidth / 2);
}