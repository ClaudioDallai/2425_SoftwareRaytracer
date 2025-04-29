#pragma once
#include "Vector3.h"
#include "Color.h"

struct Material 
{
    Color Albedo;
    float SpecularShiningFactor;
    float ReflectionFactor;
};

struct Sphere 
{
    Vector3 Center;
    float Radius;
    Material Material;

    Sphere(const Vector3& InCenter, float InRadius);
};