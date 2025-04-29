#pragma once

struct Color 
{
    float R;
    float G;
    float B;

    Color();

    Color(float InR, float InG, float InB);

    Color operator*(float InScalar) const;

    Color operator+(const Color& InOther) const;

    Color Clamp() const;
};