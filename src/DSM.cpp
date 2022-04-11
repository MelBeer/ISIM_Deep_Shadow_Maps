#include "DSM.hpp"

#define MIN_STEP 0.1f

double myRand() {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(-0.5,0.5);
    return distribution(generator);
}

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
    double v0 = 1;
    double z0 = 0;
    double v1 = 1;
    double z1 = 0;
    for (unsigned int i = 1; i < sample_zs.size(); i++)
    {
        if (sample_zs[i] > z)
        {
            v1 = sample_Vs[i];
            z1 = sample_zs[i];
            break;
        }
        z0 = sample_zs[i];
        v0 = sample_Vs[i];
    }
    // yp = y0 + ((y1-y0)/(x1-x0)) * (xp - x0);
    return v0 + ((v1-v0)/(z1-z0)) * (z - z0);
}

DSM::Visibility DSM::drawPixel(const aiVector3t<double> &refPixel, double w, double h, const aiScene &scene, const Camera &camera, const double pixelSize)
{   
    struct FaceData
    {
        double min;
        double max;
        double intersections;
    };
    
    std::unordered_map<aiFace *, FaceData> faceMap;
    auto usualPixel = refPixel + camera.right * pixelSize * w - camera.up * pixelSize * h;

    for (unsigned int i = 0; i < raysPerPixel; i++)
    {
        auto actPixel = (usualPixel) + (camera.right * pixelSize * myRand()) - (camera.up * pixelSize * myRand());
        aiVector3t<double> ray = (actPixel - camera.center).Normalize();

        aiFace *facePointer;
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
                    facePointer = &actFace;
                    faceDist = dist;
                    intersectionPt = intPt;
                }
            }
        }
        if (faceDist != camera.farClipPlane)
        {
            auto zMin = faceDist;
            auto zMax = faceDist;
            double intersections = 0;
            auto faceEntry = faceMap.find(facePointer);
            if (faceEntry != faceMap.end())
            {
                zMin = faceMap[facePointer].min;
                zMax = faceMap[facePointer].max;
                intersections = faceMap[facePointer].intersections;
            }
            auto data = FaceData();
            data.min = std::min(faceDist, zMin);
            data.max = std::max(faceDist, zMax);
            data.intersections = intersections + 1;
            faceMap[facePointer] = data;
        }
    }

    auto zs = std::vector<double>();
    auto Vs = std::vector<double>();
    double proportion = ((1 - shadowIntensity) / raysPerPixel);
    for(const auto & [ facePtr, data ] : faceMap)
    {
        const auto z0 = data.min;
        const auto z1 = data.max;
        const auto V0 = 1;
        const auto V1 = V0 - proportion * data.intersections;
        zs.push_back(z0);
        zs.push_back(z1);
        Vs.push_back(V0);
        Vs.push_back(V1);
    }

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

    auto vecOnRight = vecFromOrigin * camera.right;
    int w = (int)(vecOnRight/camera.pixelWidth);

    auto vecOnUp = vecFromOrigin * camera.up;
    int h = -(int)(vecOnUp/camera.pixelHeight);


    if (0 > h || height <= h || 0 > w || width <= w)
        return clearVisibility;
    return visibilities[h * width + w];
    
}
