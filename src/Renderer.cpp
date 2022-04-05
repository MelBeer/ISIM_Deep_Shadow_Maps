//
// Created by melbeer on 29/03/2022.
//

#include "Renderer.h"

#define DEFAULT_COLOR aiColor3D{127,127,127}
#define DEFAULT_KD 0.5
#define DEFAULT_KS 0.2

Image Renderer::renderScene(int imgWidth, int imgHeight) {
    camera.SetPixelSize(imgHeight, imgWidth);
    pixelSize = camera.planeWidth / imgWidth;
    std::cout << "planeWidth : " << camera.planeWidth << '\n';
    aiVector3t<double> actPixel = camera.originPixel + camera.right * (pixelSize/2) - camera.up * (pixelSize/2);

    std::cout << "origin : " << actPixel << '\n';

    std::vector<aiColor3D> pixValues = std::vector<aiColor3D>();
    pixValues.reserve(sizeof(aiColor3D) * imgHeight * imgWidth);
    aiColor3D pixColor = aiColor3D{0, 0, 0};

    for (int h = 0; h < imgHeight; ++h) {
        for (int w = 0; w < imgWidth; ++w) {
            aiVector3t<double> ray = (actPixel - camera.center).Normalize();
            //std::cout << "point : " << h << ", " << w << '\n';
            if ((h == 0 && w == 0) || (h == imgHeight-1 && w == imgWidth-1))
                std::cout << "ray : " << ray << '\n';

            int m = 0;
            int f = 0;
            aiVector3t<double> intersectionPt = findClosestIntersectPt(ray, f, m,
                                           camera.center);

            aiMesh *mesh = scene->mMeshes[m];
            aiFace &face = mesh->mFaces[f];

            double dist = (intersectionPt - camera.center).Length();

            if (dist > camera.nearClipPlane)
            {
                // TODO : iterate over every lights;
                auto light = scene->mLights[0];

                auto testnormal = mesh->mNormals[face.mIndices[0]];
                aiColor3D diffuse = aiColor3D(testnormal.x, testnormal.y, testnormal.z) * DEFAULT_COLOR; // DEFAULT_COLOR * DEFAULT_KD * (mesh.mNormals[face.mIndices[0]] * (light->mPosition - intersectionPt)) * light->mAttenuationConstant; // TODO : check for light intensity
                // TODO : define specular
                // aiColor3D specular =

                pixColor = diffuse;
            }
            else
                pixColor = aiColor3D{0, 0, 0};

            pixValues.emplace_back(pixColor);

            actPixel += camera.right * pixelSize;
        }
        if (h == 0)
            std::cout << "endofline : " << actPixel << '\n';
        else if (h == imgHeight - 1)
            std::cout << "endofmatrix : " << actPixel << '\n';
        actPixel -= camera.right * (pixelSize * imgWidth) + camera.up * pixelSize;
        if (h == imgHeight - 2)
            std::cout << "endofcolumn : " << actPixel << '\n';
    }

    return { imgHeight, imgWidth, pixValues };
}

void Renderer::setCamera() {
    this->camera = Camera(aiVector3t<double>(0, -5, 1), aiVector3t<double>(0, 0, 0), 80, 16.0/9.0,
                          1, 500);
    camera.SetPixelSize(1600, 900);
}

aiVector3t<double>
Renderer::findClosestIntersectPt(aiVector3t<double> ray, int &face, int &mesh, aiVector3t<double> nullvalue) {
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
                /*if (f != 0 && f != 44 && f != 70 && f!= 71)
                    std::cout << "f : " << f << std::endl;*/
            }
        }
    }
    //if (nb_intersects > 0)
    //std::cout << "nb_intersects : " << nb_intersects << "for ray going to" << ray << '\n';

    return intersectionPt;
}
