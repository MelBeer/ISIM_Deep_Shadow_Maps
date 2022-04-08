//
// Created by melbeer on 29/03/2022.
//

#include "Renderer.h"

#define DEFAULT_COLOR aiColor3D{127,127,127}
#define DEFAULT_KD 0.5
#define DEFAULT_KS 0.2

void spinThread(const Renderer *renderer, const aiVector3t<double> *refPixel, int h, Image *image)
{
        for (double w = 0 ; w < image->width; ++w)
        {
            renderer->drawPixel(*refPixel, w, h, *image);
        }
}

void Renderer::drawPixel(const aiVector3t<double> &refPixel, double w, double h, Image &image) const
{

    aiColor3D pixColor = aiColor3D{0, 0, 0};
    auto actPixel = refPixel + camera.right * pixelSize * w - camera.up * pixelSize * h;  
    aiVector3t<double> ray = (actPixel - camera.center).Normalize();

    int m = 0;
    int f = 0;
    aiVector3t<double> intersectionPt = findClosestIntersectPt(ray, f, m, camera.center);

    aiMesh *mesh = scene->mMeshes[m];
    aiFace &face = mesh->mFaces[f];

    double dist = (intersectionPt - camera.center).Length();

    if (dist > camera.nearClipPlane)
    {
        // TODO : iterate over every lights;
        auto light = scene->mLights[0];

        auto testnormal = mesh->mNormals[face.mIndices[0]];
        aiColor3D diffuse = aiColor3D(testnormal.x, testnormal.y, testnormal.z) * DEFAULT_COLOR; // aiColor3D(125, 125, 125); // DEFAULT_COLOR * DEFAULT_KD * (mesh.mNormals[face.mIndices[0]] * (light->mPosition - intersectionPt)) * light->mAttenuationConstant; // TODO : check for light intensity
        // TODO : define specular
        // aiColor3D specular =

        pixColor = diffuse;
    }
    else
        pixColor = aiColor3D{0, 0, 0};

    // std::cout << image.getIndex(h,w)<< "," << std::endl;
    image.pixels[image.getIndex(h,w)] = pixColor;
}

Image Renderer::renderScene(int imgWidth, int imgHeight) {
    camera.SetPixelSize(imgHeight, imgWidth);
    pixelSize = camera.planeWidth / imgWidth;
    std::cout << "planeWidth : " << camera.planeWidth << '\n';
    aiVector3t<double> refPixel = camera.originPixel + camera.right * (pixelSize/2) - camera.up * (pixelSize/2);
    

    std::cout << "forward : " << camera.forward << '\n';

    std::vector<aiColor3D> pixValues = std::vector<aiColor3D>(imgWidth * imgHeight);
    // pixValues.reserve(sizeof(aiColor3D) * imgHeight * imgWidth);
    Image image(imgHeight, imgWidth, pixValues);
    

    auto startpoint = std::chrono::high_resolution_clock::now();

    auto threads = std::vector<std::thread>();
    threads.reserve(imgHeight);
    for (double h = 0; h < imgHeight; ++h) {
        std::thread thread(spinThread, this, &refPixel, h, &image);
        threads.push_back(std::move(thread));
    }

    for(auto &thread: threads) {
        thread.join();
    }


    auto stoppoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startpoint).time_since_epoch().count();
    auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(stoppoint).time_since_epoch().count();

    auto duration = stop - start;
    std::cout << "Rendering latency: " << duration << " microseconds" << std::endl;

    return image;
}

void Renderer::setCamera() {
    this->camera = Camera(aiVector3t<double>(1200, 1200, 500), aiVector3t<double>(-100, 1000, 50), 80, 16.0/9.0,
                          1, 500000000);
}

aiVector3t<double>
Renderer::findClosestIntersectPt(aiVector3t<double> ray, int &face, int &mesh, aiVector3t<double> nullvalue)  const {
    double faceDist = camera.farClipPlane;
    aiVector3t<double> intersectionPt = nullvalue;
    int nb_intersects = 0;
    for (int m = 0; m < scene->mNumMeshes; ++m) {
        auto actMesh = scene->mMeshes[m];

        for (int f = 0; f < actMesh->mNumFaces; ++f) {
            auto actFace = actMesh->mFaces[f];

            aiVector3t<double> intPt = planeIntersect(actFace, *actMesh, camera.center, ray, nullvalue);

            double dist = (intPt - camera.center).Length();
            if (dist > camera.nearClipPlane && dist < faceDist)
            {
                nb_intersects++;
                faceDist = dist;
                intersectionPt = intPt;
                face = f;
                mesh = m;
            }
        }
    }

    return intersectionPt;
}
