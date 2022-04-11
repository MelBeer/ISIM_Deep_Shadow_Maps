//
// Created by melbeer on 29/03/2022.
//

#include "Renderer.h"

#define DEFAULT_COLOR aiVector3t<double>{127,127,127}
#define DEFAULT_KD 0.5
#define DEFAULT_KS 0.05
#define NS 15

void spinThread(const Renderer *renderer, const aiVector3t<double> *refPixel, int h, Image *image)
{
        for (double w = 0 ; w < image->width; ++w)
        {
            renderer->drawPixel(*refPixel, w, h, *image);
        }
}

void Renderer::drawPixel(const aiVector3t<double> &refPixel, double w, double h, Image &image) const
{

    aiVector3t<double> pixColor = aiVector3t<double>{0, 0, 0};
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

            aiVector3t<double> normal = mesh->mNormals[face.mIndices[0]];
            auto lightVec = light.position - intersectionPt;
            double lightDistance = lightVec.Length();
            lightVec = lightVec.Normalize();

            const auto dsmVis = dsm.visibilityFromPoint(intersectionPt);
            //std::cout << "Vis func = " << dsmVis.function(dist) << std::endl;
            aiVector3t<double> diffuse = DEFAULT_COLOR * DEFAULT_KD * std::fabs(normal * lightVec) * (light.intensity / lightDistance) * dsmVis.function(dist);

            auto reflected = (ray - 2 * (normal * ray) * normal).Normalize();
            auto specular = DEFAULT_COLOR * DEFAULT_KS * (light.intensity / lightDistance) * pow(reflected * lightVec, NS);

            pixColor = diffuse + specular;
            pixColor.x = pixColor.x > 255 ? 255 : pixColor.x < 0 ? 0 : pixColor.x;
            pixColor.y = pixColor.y > 255 ? 255 : pixColor.y < 0 ? 0 : pixColor.y;
            pixColor.z = pixColor.z > 255 ? 255 : pixColor.z < 0 ? 0 : pixColor.z;
        }
        else
            pixColor = aiVector3t<double>{0, 0, 0};

    image.pixels[image.getIndex(h,w)] = pixColor;
}

Image Renderer::renderScene(int imgWidth, int imgHeight) {
    camera.SetPixelSize(imgHeight, imgWidth);
    pixelSize = camera.planeWidth / imgWidth;
    std::cout << "planeWidth : " << camera.planeWidth << '\n';
    aiVector3t<double> refPixel = camera.originPixel + camera.right * (pixelSize/2) - camera.up * (pixelSize/2);
    

    std::cout << "forward : " << camera.forward << '\n';

    auto pixValues = std::vector<aiVector3t<double>>(imgWidth * imgHeight);
    // pixValues.reserve(sizeof(aiColor3D) * imgHeight * imgWidth);
    Image image(imgHeight, imgWidth, pixValues);
    
    auto dsmcam = DSM::defaultCameraFromPointLight(light);
    dsm.drawMap(dsmcam, *scene);

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
    this->camera = Camera(aiVector3t<double>(4, -4, 4), aiVector3t<double>(0, 0, 2), 80, 16.0/9.0,
                          1, 500);
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
