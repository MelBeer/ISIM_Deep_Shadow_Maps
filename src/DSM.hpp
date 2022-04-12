#pragma once

#include <cassert>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "geometry.h"
#include "Camera.h"
#include "PointLight.h"

#define SHADOW 0.2f
#define DRAWLIMIT 5000000

class DSM
{
public:
    class Visibility
    {
        public:
        struct Point
        {
            double z;
            double v;
        };
        private:
            
        public:
            std::vector<Point> samples;
            Visibility()
            : samples(std::vector<Point>())
            {
                samples.push_back({0, 1});
                samples.push_back({DRAWLIMIT, 1});
            }
            
            void compress();
            void addVisibilityLoss(double z0, double z1, double V1);
            double function(const double z) const ;
            unsigned int size() const { return samples.size(); };
            struct CompareZ {
                bool operator()(const double &val, const Point &item) const {return val > item.z;}
                bool operator()(const Point &item, const double &val) const {return val > item.z;}
                bool operator()(const Point &a, const Point &b) const {return a.z < b.z;}
            };
            struct CompareV {
                bool operator()(const double &val, const Point &item) const {return val > item.v;}
                bool operator()(const Point &item, const double &val) const {return val > item.v;}
            };
            friend std::ostream &operator<<(std::ostream &os, const Visibility &visibility)
            {
                os << "Visibility of size :" << visibility.size() << "[\n";

                for (auto pt: visibility.samples)
                {
                    os << "z:" << pt.z << ",v:" << pt.v << "\n";
                }
                os << "]";
                return os;
            }
    };

    const unsigned int height;
    const unsigned int width;
    const unsigned int size;
    const unsigned int raysPerPixel = 4;
    

    Camera camera;

    Visibility clearVisibility = Visibility();

    DSM(unsigned int height, unsigned int width)
    : height(height), width(width), size(height * width)
    {
        visibilities = std::vector<Visibility>(size);
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