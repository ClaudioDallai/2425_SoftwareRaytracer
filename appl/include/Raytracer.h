#pragma once
#include "Color.h"
#include "Vector3.h"
#include "Scene.h"

struct Ray {
    Vector3 Origin;
    Vector3 Direction;
};

struct RayHit {
    Vector3 Point;
    const Sphere* Object;
    float Distance;
    Vector3 Normal;
};

class Raytracer 
{
public:
    Color Raytrace(const Ray& InRay, const Scene& InScene, int InCurrentDepth);
};