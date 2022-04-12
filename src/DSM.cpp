#include "DSM.hpp"

#define MIN_STEP 0.1f

double myRand() {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(-0.5,0.5);
    return distribution(generator);
}

double interpolation(const double z, const double z0, const double v0, const double z1, const double v1)
{
    // if (z < z0)
    //     return v0;
    // if (z > z1)
    //     return v1;
    auto v = (v0 + ((v1-v0)/(z1-z0)) * (z - z0));
    if (v > 1)
    {
        std::cout << "v0:" << v0 << "|v1:" << v1 << "|v:"<< v <<"|z0:" << z0 << "|z1:" << z1 << "|z:" << z << std::endl;
        abort();
    }
    return v;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;  
    }
};

void DSM::Visibility::addVisibilityLoss(const double z0, const double z1, const double initial_V1)
{
    if (this->samples.size() == 0)
    {
        samples.push_back({z0, 1});
        samples.push_back({z1,initial_V1});
        return;
    }
    
    const auto upper_bound_z0 = std::upper_bound(this->samples.begin(), this->samples.end(),
                                                  z0, DSM::Visibility::CompareZ());
    const auto lower_bound_z1 = std::lower_bound(this->samples.begin(), this->samples.end(),
                                                  z1, DSM::Visibility::CompareZ());
    const auto upper_bound_z1 = std::upper_bound(this->samples.begin(), this->samples.end(),
                                                  z1, DSM::Visibility::CompareZ());
    const auto lower_bound_z0 = std::lower_bound(this->samples.begin(), this->samples.end(),
                                                  z0, DSM::Visibility::CompareZ());
    
    const double V_diff = 1 - initial_V1;
    double V0 = 1;
    double V1 = initial_V1;
    
    // Modification des éléments ajoutés 
    V0 = interpolation(z0,
                        (lower_bound_z0)->z,
                        (lower_bound_z0)->v,
                        (upper_bound_z0)->z,
                        (upper_bound_z0)->v
                        );
    
    
    if (lower_bound_z1 != this->samples.begin())
    {
        V1 = interpolation(z1,
                        (lower_bound_z1)->z,
                        (lower_bound_z1)->v,
                        (upper_bound_z1)->z,
                        (upper_bound_z1)->v
                        ) - V_diff;
        if (V1 > 1)
        {
            std::cout << "added elt";
            abort();
        }
    }
    else
    {
        V1 = (upper_bound_z1)->v - V_diff;
        if (V1 > 1)
        {
            std::cout << "added elt2";
            abort();
        }
    }
    
    // Modification des éléments existants


    for (auto it = upper_bound_z0; it < lower_bound_z1; it++)
    {
        it->v -= (1 - interpolation(it->z,
                          z0,
                          1,
                          z1,
                          initial_V1));
        if (it->v > 1)
        {
            std::cout << "modif pre";
            abort();
        }
    }

    
    for (auto it = upper_bound_z1; it <  this->samples.end(); it++)
    {
        it->v -= V_diff;
        if (it->v > 1)
        {
            std::cout << "modif post";
            abort();
        }
    }
    

    this->samples.insert(upper_bound_z0, {z0, V0});
    const auto new_lower_bound_it1 = std::lower_bound(this->samples.begin(), this->samples.end(),
                                       z1, DSM::Visibility::CompareZ());
    this->samples.insert(new_lower_bound_it1, {z1, V1});


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
    if (samples.size() == 0)
        return 1;
    if (samples.back().z < z)
        return samples.back().v;
    const auto pt0 = std::lower_bound(this->samples.begin(), this->samples.end(), z, DSM::Visibility::CompareZ());
    const auto pt1 = std::upper_bound(this->samples.begin(), this->samples.end(), z, DSM::Visibility::CompareZ());
    const auto v0 = pt0->v;
    const auto v1 = pt1->v;
    const auto z0 = pt0->z;
    const auto z1 = pt1->z;
    const auto v = interpolation(z, z0, v0, z1, v1);
    return v;
}

DSM::Visibility DSM::drawPixel(const aiVector3t<double> &refPixel, double w, double h, const aiScene &scene, const Camera &camera, const double pixelSize)
{   
    struct FaceData
    {
        double min;
        double max;
        double intersections;
    };
    
    std::unordered_map<std::pair<int, int>, FaceData, pair_hash> faceMap;
    auto usualPixel = refPixel + camera.right * pixelSize * w - camera.up * pixelSize * h;

    for (unsigned int i = 0; i < raysPerPixel; i++)
    {
        auto actPixel = (usualPixel) + (camera.right * pixelSize * myRand()) - (camera.up * pixelSize * myRand());
        aiVector3t<double> ray = (actPixel - camera.center).Normalize();

        double faceDist = camera.farClipPlane;
        aiVector3t<double> intersectionPt = camera.center;
        std::pair<int, int> mf;

        for (int m = 0; m < scene.mNumMeshes; ++m) {
            auto actMesh = scene.mMeshes[m];

            for (int f = 0; f < actMesh->mNumFaces; ++f) {
                auto actFace = actMesh->mFaces[f];

                aiVector3t<double> intPt = planeIntersect(actFace, *actMesh, camera.center, ray, camera.center);

                double dist = (intPt - camera.center).Length();
                if (dist > camera.nearClipPlane && dist < faceDist)
                {
                    mf.first = m;
                    mf.second = f;
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
            auto faceEntry = faceMap.find(mf);
            if (faceEntry != faceMap.end())
            {
                zMin = faceMap[mf].min;
                zMax = faceMap[mf].max;
                intersections = faceMap[mf].intersections;
            }
            auto data = FaceData();
            data.min = std::min(faceDist, zMin);
            data.max = std::max(faceDist, zMax);
            data.intersections = intersections + 1;
            faceMap[mf] = data;
        }
    }
    auto visibility = Visibility();
    double proportion = 1 / raysPerPixel;
    for(const auto & bla : faceMap)
    {
        const auto data = bla.second;
        const auto z0 = data.min;
        const auto z1 = data.max;
        const auto V1 = 1 - (data.intersections / raysPerPixel);
        visibility.addVisibilityLoss(z0,z1,V1);
    }

    return visibility;
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
