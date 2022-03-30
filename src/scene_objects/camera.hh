#pragma once

#include <cmath>
#include "point3.hh"
#include "vector3.hh"


class Camera {
    private : 
        Point3 imagePlanPosition; 
        Point3 originPixel; 
        float planeWidth; 
        float planeHeight; 
        float pixelHeight = 0;
        float pixelWidth = 0;  

    public : 
     Point3 center; 
     Point3 lookAt; 
     Vector3 up; 
     Vector3 forward; 
     Vector3 right; 
     float fovAlpha;
     float fovBeta; 
     float nearClipPlane; 

     Camera() {

     };

     Camera(Point3 center, Point3 lookAt, Vector3 up, float fovAlpha, float fovBeta, float nearClipPlane); 

     void SetPixelSize(int height, int width); 

     Point3 GetPixelPos(int posH, int posW); 

};