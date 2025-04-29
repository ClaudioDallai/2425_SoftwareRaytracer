#pragma once
#include "SDL.h"
#include <vector>
#include "Sphere.h"
#include "Light.h"

class Scene 
{
public:
    Scene(int w, int h, SDL_Renderer* r);
    ~Scene();
    void Update(float delta_time);

public:    
    std::vector<Sphere> Spheres;
    Light Light;
    Color BackgroundColor;

private:
    int Width;
    int Height;
    SDL_Renderer* Renderer;


};