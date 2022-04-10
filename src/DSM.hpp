#pragma once

#include <vector>
#include <algorithm>
#include "Camera.h"
#include "PointLight.h"

#define DSM_SAMPLES 20000

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
    DSM(unsigned int height, unsigned int width)
    : height(height), width(width), size(height * width)
    {
        visibilities = std::vector<Visibility>();
    }
    static Camera defaultCameraFromPointLight(PointLight pointLight);
    void drawMap(Camera camera);
    Visibility visibilityAt(unsigned int h, unsigned int w) const;
    private:
        std::vector<Visibility> visibilities;
    
private:
    Visibility drawPixel(double w, double h, Camera camera);

};