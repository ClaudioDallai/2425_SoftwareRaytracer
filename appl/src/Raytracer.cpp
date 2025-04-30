#include "Raytracer.h"
#include <cmath>
#include <limits>

static bool RayCast(const Ray& InRay, const Scene& InScene, RayHit& OutHit);
static bool RaySphereIntersection(const Ray& InRay, const Sphere& InSphere, RayHit& OutHit);

Color Raytracer::Raytrace(const Ray& InRay, const Scene& InScene) 
{   
    RayHit Hit;
    bool HasHit = RayCast(InRay, InScene, Hit);
    if (!HasHit) return InScene.BackgroundColor;
    return Hit.Object->Material.Albedo;
}

bool RayCast(const Ray& InRay, const Scene& InScene, RayHit& OutHit)
{
    RayHit BestHit;
    BestHit.Distance = std::numeric_limits<float>::max();

    bool HasBestHit = false;

    for(auto& Sphere : InScene.Spheres)
    {
        RayHit Hit;
        bool HasHit = RaySphereIntersection(InRay, Sphere, Hit);

        if (HasHit && Hit.Distance < BestHit.Distance)
        {
            BestHit = Hit;
            HasBestHit = true;
        }
    }

    if (HasBestHit) 
    {
        OutHit = BestHit;
    }

    return HasBestHit;
}

bool RaySphereIntersection(const Ray& InRay, const Sphere& InSphere, RayHit& OutHit)
{
    Vector3 L = InSphere.Center - InRay.Origin;
    float Tca = L.Dot(InRay.Direction);

    float MagnL2 = L.MagnitudeSquared();
    float D2 = MagnL2 - Tca * Tca;
    float Radius2 = InSphere.Radius * InSphere.Radius;
    if (D2 > Radius2) return false;

    float Thc = sqrtf(Radius2 - D2);

    float T0 = Tca - Thc;
    float T1 = Tca + Thc;

    if (T1 < T0) 
    {
        float Temp = T0;
        T0 = T1;
        T1 = Temp;
    }

    if (T0 < 0) 
    {
        T0 = T1;
        if (T0 < 0) return false;
    }

    OutHit.Distance = T0;
    OutHit.Object = &InSphere;
    OutHit.Point = InRay.Origin + InRay.Direction * T0;
    return true;
}