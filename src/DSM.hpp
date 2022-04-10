#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include "geometry.h"
#include "Camera.h"
#include "PointLight.h"

#define SAMPLE_STEP 0.0001f

class DSM
{
public:
    class Visibility
    {
        private:
            std::vector<double> sample_zs;
            std::vector<double> sample_Vs;
        public:
            Visibility(const std::vector<double> &sample_zs, const std::vector<double> &sample_Vs)
            : sample_zs(sample_zs), sample_Vs(sample_Vs)
            {}
            void compress();
            double function(const double z) const ;
    };

    const unsigned int height;
    const unsigned int width;
    const unsigned int size;
    Camera camera;

    Visibility clearVisibility = Visibility(std::vector<double>(0), std::vector<double>(0));

    DSM(unsigned int height, unsigned int width)
    : height(height), width(width), size(height * width)
    {
        visibilities = std::vector<Visibility>();
    }

    static Camera defaultCameraFromPointLight(PointLight pointLight);
    void drawMap(Camera &camera, const aiScene &scene);
    Visibility visibilityAt(unsigned int h, unsigned int w) const;
    Visibility visibilityFromPoint(aiVector3t<double> pos) const;
    private:
        std::vector<Visibility> visibilities;
    
private:
    Visibility drawPixel(const aiVector3t<double> &refPixel, double w, double h, const aiScene &scene, const Camera &camera, const double pixelsize);
    void spinThread(const aiVector3t<double> *refPixel, const aiScene *scene, double h, const Camera *camera, const double pixelSize);
};