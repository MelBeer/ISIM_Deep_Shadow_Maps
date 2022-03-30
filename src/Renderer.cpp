//
// Created by melbeer on 29/03/2022.
//

#include "Renderer.h"
#include "geometry.h"
#include "Image.h"

#define DEFAULT_COLOR aiColor3D{127,127,127}
#define DEFAULT_KD 0.5
#define DEFAULT_KS 0.2

Image Renderer::renderScene(int imgWidth) {
    int imgHeight = imgWidth / ratio;
    pixelSize = clipPlaneWidth / imgWidth;
    aiVector3t<float> actPixel = originPixel + camRight * (pixelSize/2) - camera->mUp * (pixelSize/2);

    std::vector<aiColor3D> pixValues = std::vector<aiColor3D>();
    pixValues.reserve(sizeof(aiColor3D) * imgHeight * imgWidth);
    aiColor3D pixColor = aiColor3D{0, 0, 0};

    for (int h = 0; h < imgHeight; ++h) {
        for (int w = 0; w < imgWidth; ++w) {
            aiVector3t<float> ray = actPixel - camera->mPosition;

            aiMesh mesh;
            aiFace face;
            aiVector3t<float> intersectionPt = findClosestIntersectPt(ray, face, mesh,
                                                                      camera->mPosition);

            float dist = (intersectionPt - camera->mPosition).Length();

            if (dist > camera->mClipPlaneNear)
            {
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

            actPixel += camRight * pixelSize;
        }

        actPixel -= camRight * (pixelSize * imgWidth) + camera->mUp * pixelSize;
    }

    return { imgHeight, imgWidth, pixValues };
}

void Renderer::setCamera(uint nCamera) {
    this->camera = scene->mCameras[nCamera];
    this->camRight = camera->mUp ^ camera->mLookAt;
    this->clipPlaneWidth = camera->mClipPlaneNear * tan((camera->mHorizontalFOV / 2.0) * M_PI / 180.0) * 2;
    this->clipPlaneHeight = clipPlaneWidth / ratio;
    this->originPixel = camera->mPosition + camera->mLookAt * camera->mClipPlaneNear - camRight * (clipPlaneWidth/2) + camera->mUp * (clipPlaneHeight/2);
}

aiVector3t<float>
Renderer::findClosestIntersectPt(aiVector3t<float> ray, aiFace &face, aiMesh &mesh, aiVector3t<float> nullvalue) {
    float faceDist = camera->mClipPlaneFar;
    aiVector3t<float> intersectionPt = nullvalue;
    for (int m = 0; m < scene->mNumMeshes; ++m) {
        auto actMesh = scene->mMeshes[m];

        for (int f = 0; f < actMesh->mNumFaces; ++f) {
            auto actFace = actMesh->mFaces[f];

            aiVector3t<float> intPt = planeIntersect(&actFace, actMesh, &camera->mPosition, &ray, nullvalue);

            float dist = (intPt - camera->mPosition).Length();
            if (dist > camera->mClipPlaneNear && dist < faceDist)
            {
                faceDist = dist;
                intersectionPt = intPt;
                face = actFace;
                mesh = *actMesh;
            }
        }
    }

    return intersectionPt;
}
