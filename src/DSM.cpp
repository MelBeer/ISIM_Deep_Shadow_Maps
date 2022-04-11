#include "DSM.hpp"

#define MIN_STEP 0.1f


void DSM::spinThread(const aiVector3t<double> *refPixel, const aiScene *scene, double h, const Camera *camera, const double pixelSize)
{
        for (double w = 0 ; w < width; ++w)
        {
            visibilities[h * width + w] = this->drawPixel(*refPixel, w, h, *scene, *camera, pixelSize);
        }
}

// Binary search then 
double DSM::Visibility::function(const double z) const
{
    if (sample_zs.size() == 0)
        return 1;
    if (sample_zs.back() < z)
        return sample_Vs.back();
    double left_v = 1;
    auto v_at_first_higher_z = std::upper_bound(sample_zs.begin(),sample_zs.end(), z);
    if (v_at_first_higher_z != sample_zs.begin())
        left_v = *(v_at_first_higher_z - 1);
    const double right_v = *v_at_first_higher_z;
    return (left_v + right_v) / 2;
}

DSM::Visibility DSM::drawPixel(const aiVector3t<double> &refPixel, double w, double h, const aiScene &scene, const Camera &camera, const double pixelSize)
{
    auto actPixel = refPixel + camera.right * pixelSize * w - camera.up * pixelSize * h;
    aiVector3t<double> ray = (actPixel - camera.center).Normalize();
    
    auto zs = std::vector<double>();
    auto Vs = std::vector<double>();

    double faceDist = camera.farClipPlane;
    aiVector3t<double> intersectionPt = camera.center;
    for (int m = 0; m < scene.mNumMeshes; ++m) {
        auto actMesh = scene.mMeshes[m];

        for (int f = 0; f < actMesh->mNumFaces; ++f) {
            auto actFace = actMesh->mFaces[f];

            aiVector3t<double> intPt = planeIntersect(actFace, *actMesh, camera.center, ray, camera.center);

            double dist = (intPt - camera.center).Length();
            if (dist > camera.nearClipPlane && dist < faceDist)
            {
                faceDist = dist;
                intersectionPt = intPt;
            }
        }
    }

    zs.push_back(faceDist);
    Vs.push_back(1);
    zs.push_back(faceDist + 1);
    Vs.push_back(0.2);
    return Visibility(zs, Vs);
}

void DSM::drawMap(Camera &cam, const aiScene &scene)
{
    this->camera = cam;
    camera.SetPixelSize(height, width);
    double pixelSize = camera.planeWidth / width;

    aiVector3t<double> refPixel = camera.originPixel + camera.right * (pixelSize/2) - camera.up * (pixelSize/2);

    auto startpoint = std::chrono::high_resolution_clock::now();

    auto threads = std::vector<std::thread>();
    threads.reserve(height);
    for (double h = 0; h < height; ++h) {
        std::thread thread(&DSM::spinThread, this, &refPixel, &scene, h, &camera, pixelSize);
        threads.push_back(std::move(thread));
    }

    for(auto &thread: threads) {
        thread.join();
    }


    auto stoppoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startpoint).time_since_epoch().count();
    auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(stoppoint).time_since_epoch().count();

    auto duration = stop - start;
    std::cout << "Shadowmapping latency: " << duration << " microseconds" << std::endl;
}

void DSM::Visibility::compress()
{}

Camera DSM::defaultCameraFromPointLight(PointLight pointLight)
{
    return {pointLight.position, aiVector3t<double>(0, 0, 0), 80, 1/1,
                1, 5000};
}
DSM::Visibility DSM::visibilityFromPoint(aiVector3t<double> pos) const {
    auto pointOnImagePlan = camera.center + (pos - camera.center).Normalize() * camera.nearClipPlane;
    auto vecFromOrigin = pointOnImagePlan - camera.originPixel;

    //Thales Theorem
    auto rayToPoint = (pos - camera.center).Normalize();
    auto rayOnRight = rayToPoint * camera.right;
    auto rightDepl = (camera.nearClipPlane * rayOnRight) / (rayToPoint * camera.forward);
    auto rayOnUp = rayToPoint * camera.up;
    auto upDepl = (camera.nearClipPlane * rayOnUp) / (rayToPoint * camera.forward);

    //std::cout << vecFromOrigin * camera.right << std::endl;

    auto vecOnRight = vecFromOrigin * camera.right;
    int w = (int)(vecOnRight/camera.pixelWidth); //width/2 + (int)(rightDepl/camera.pixelWidth);

    auto vecOnUp = vecFromOrigin * camera.up;
    int h = -(int)(vecOnUp/camera.pixelHeight); //height/2 + (int)(upDepl/camera.pixelHeight);

    std::cout << h << " | " << w  << " | " << h * width + w << std::endl;

    if (0 > h || height <= h || 0 > w || width <= w)
        return clearVisibility;
    return visibilities[h * width + w];
    
}
