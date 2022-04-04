//
// Created by melbeer on 29/03/2022.
//

#include "Renderer.h"

#define DEFAULT_COLOR aiColor3D{127,127,127}
#define DEFAULT_KD 0.5
#define DEFAULT_KS 0.2

Image Renderer::renderScene(int imgWidth) {
    int imgHeight = imgWidth / ratio;
    camera.SetPixelSize(imgHeight, imgWidth);
    pixelSize = camera.planeWidth / imgWidth;
    aiVector3t<float> actPixel = camera.originPixel + camera.right * (pixelSize/2) - camera.up * (pixelSize/2);

    std::vector<aiColor3D> pixValues = std::vector<aiColor3D>();
    pixValues.reserve(sizeof(aiColor3D) * imgHeight * imgWidth);
    aiColor3D pixColor = aiColor3D{0, 0, 0};

    for (int h = 0; h < imgHeight; ++h) {
        for (int w = 0; w < imgWidth; ++w) {
            aiVector3t<float> ray = aiVector3t<float>(-7,-7,-4).Normalize();//(actPixel - camera.center).Normalize();
            //std::cout << "point : " << h << ", " << w << '\n';
            //std::cout << "ray : " << ray << '\n';

            aiMesh mesh;
            aiFace face;
            aiVector3t<float> intersectionPt = findClosestIntersectPt(ray, face, mesh,
                                                                      camera.center);

            float dist = (intersectionPt - camera.center).Length();

            if (dist > camera.nearClipPlane)
            {
                std::cout << "\t intersects\n";
                // TODO : iterate over every lights;
                auto light = scene->mLights[0];

                aiColor3D diffuse = DEFAULT_COLOR * DEFAULT_KD * (mesh.mNormals[face.mIndices[0]] * (light->mPosition - intersectionPt)) * light->mAttenuationConstant; // TODO : check for light intensity
                // TODO : define specular
                // aiColor3D specular =

                pixColor = diffuse;
            }
            else
                pixColor = aiColor3D{0, 0, 0};

            pixValues.emplace_back(pixColor);

            actPixel += camera.right * pixelSize;
        }

        actPixel -= camera.right * (pixelSize * imgWidth) + camera.up * pixelSize;
    }

    return { imgHeight, imgWidth, pixValues };
}

void Renderer::setCamera() {
    this->camera = Camera(aiVector3t<float>(7, 7, 5), aiVector3t<float>(0, 0, 1), aiVector3t<float>(-4, -4, 7), 80, 45,
            0.1, 500);
    camera.SetPixelSize(1600, 900);
}

aiVector3t<float>
Renderer::findClosestIntersectPt(aiVector3t<float> ray, aiFace &face, aiMesh &mesh, aiVector3t<float> nullvalue) {
    float faceDist = camera.farClipPlane;
    aiVector3t<float> intersectionPt = nullvalue;
    for (int m = 0; m < scene->mNumMeshes; ++m) {
        auto actMesh = scene->mMeshes[m];

        int nb_intersects = 0;
        for (int f = 0; f < actMesh->mNumFaces; ++f) {
            auto actFace = actMesh->mFaces[f];

            aiVector3t<float> intPt = planeIntersect(&actFace, actMesh, &camera.center, &ray, nullvalue);

            float dist = (intPt - camera.center).Length();
            if (dist > camera.nearClipPlane && dist < faceDist)
            {
                nb_intersects++;
                faceDist = dist;
                intersectionPt = intPt;
                face = actFace;
                mesh = *actMesh;
            }
        }
        if (nb_intersects > 0)
            std::cout << "nb_intersects : " << nb_intersects << '\n';
    }

    return intersectionPt;
}
