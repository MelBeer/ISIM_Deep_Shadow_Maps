#include "DSM.hpp"

// Binary search then 
double DSM::Visibility::function(const double z) const
{
    double left_v = 1;
    auto v_at_first_higher_z = std::upper_bound(sample_zs.begin(),sample_zs.end() - 1, z);
    if (v_at_first_higher_z != sample_zs.begin())
        left_v = *(v_at_first_higher_z - 1);
    const double right_v = *v_at_first_higher_z;
    return (left_v + right_v) / 2;
}

void DSM::Visibility::compress()
{}

Camera DSM::defaultCameraFromPointLight(PointLight pointLight)
{
    return Camera(pointLight.position, aiVector3t<double>(0, 0, 0), 80, 1/1,
                1, 500000);
}

DSM::Visibility DSM::visibilityFromPoint(aiVector3t<double> pos) const {
    auto pointOnImagePlan = camera.center + (pos - camera.center).Normalize() * camera.nearClipPlane;
    auto vecToCenter = camera.imagePlanPosition - pointOnImagePlan;

    auto vecOnRight = vecToCenter * camera.right;
    unsigned int w = width/2 - vecOnRight/camera.pixelWidth;

    auto vecOnUp = vecToCenter * camera.up;
    unsigned int h = height/2 - vecOnUp/camera.pixelHeight;

    return visibilities[h * width + w];
}
