#pragma once

#include <cmath>
#include "point3.hh"
#include "vector3.hh"


class Camera {
    private : 
        Point3 imagePlanPosition; 
        Point3 originPixel; 
        double planeWidth; 
        double planeHeight; 
        double pixelHeight = 0;
        double pixelWidth = 0;  

    public : 
     Point3 center; 
     Point3 lookAt; 
     Vector3 up; 
     Vector3 forward; 
     Vector3 right; 
     double fovAlpha;
     double fovBeta; 
     double nearClipPlane; 

     Camera() {

     };

     Camera(aiVector3t<double> center, aiVector3t<double> lookAt, aiVector3t<double> up, double fovAlpha, double fovBeta,
            double nearClipPlane, double farClipPlane);

     void SetPixelSize(int height, int width); 

     Point3 GetPixelPos(int posH, int posW); 

};