#include "Raytracer.h"
#include <cmath>
#include <limits>

enum class ERayCastStrategy 
{
    RAYCAST_BEST,
    RAYCAST_FIRST
};

static bool RayCast(const Ray& InRay, const Scene& InScene, ERayCastStrategy InStrategy, RayHit& OutHit);
static bool RaySphereIntersection(const Ray& InRay, const Sphere& InSphere, RayHit& OutHit);

Color Raytracer::Raytrace(const Ray& InRay, const Scene& InScene, int InCurrentDepth)
{   
    if (InCurrentDepth > 3) return InScene.BackgroundColor;

    RayHit Hit;
    bool HasHit = RayCast(InRay, InScene, ERayCastStrategy::RAYCAST_BEST, Hit);
    if (!HasHit) return InScene.BackgroundColor;
    //return Hit.Object->Material.Albedo;

    Vector3 InvertedLightDirection = InScene.Light.Direction * -1.f;

    static float Bias = 1e-4;

    Ray ShadowRay;
    ShadowRay.Origin = Hit.Point + (Hit.Normal * Bias);
    ShadowRay.Direction = InvertedLightDirection;

    RayHit ShadowHit;
    bool HasShadowHit = RayCast(ShadowRay, InScene, ERayCastStrategy::RAYCAST_FIRST, ShadowHit);
    //if (HasShadowHit) return InScene.BackgroundColor;

    Color SphereColor = Hit.Object->Material.Albedo;

    float AmbientFactor = 0.1f;
    Color Ambient = SphereColor * AmbientFactor;

    float Lambert = fmaxf(0, InvertedLightDirection.Dot(Hit.Normal));
    Color Diffuse = (SphereColor + InScene.Light.Color) * Lambert;

    Vector3 R = InScene.Light.Direction.Reflect(Hit.Normal);
    Vector3 E = (Hit.Point * -1.f).Normalized();
    float SpecularFactor = fmaxf(0, R.Dot(E));
    float SpecularIntensity = powf(SpecularFactor, Hit.Object->Material.SpecularShiningFactor);
    Color Specular = InScene.Light.Color * SpecularIntensity;

    Color Phong = Ambient + Diffuse + Specular;
    Color FinalColor = Phong;

    if (Hit.Object->Material.ReflectionFactor > 0.f) 
    {
        Ray SecondRay;
        SecondRay.Origin = Hit.Point + (Hit.Normal * Bias);
        SecondRay.Direction = InRay.Direction.Reflect(Hit.Normal);

        Color ReflectionColor = Raytrace(SecondRay, InScene, InCurrentDepth + 1);

        FinalColor = FinalColor + ReflectionColor * Hit.Object->Material.ReflectionFactor;
    }

    FinalColor = FinalColor.Clamp();
    return FinalColor;
}

bool RayCast(const Ray& InRay, const Scene& InScene, ERayCastStrategy InStrategy, RayHit& OutHit)
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

            if (InStrategy == ERayCastStrategy::RAYCAST_FIRST) break;
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
    OutHit.Normal = (OutHit.Point - InSphere.Center).Normalized();
    return true;
}