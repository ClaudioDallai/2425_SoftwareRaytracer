#include "Color.h"

Color::Color() :
    Color(0, 0, 0) {}

Color::Color(float InR, float InG, float InB)
    : R(InR), G(InG), B(InB) {}

Color Color::operator*(float InScalar) const
{
    return {R * InScalar, G * InScalar, B * InScalar};
}

Color Color::operator+(const Color& InOther) const
{
    return {R + InOther.R, G + InOther.G, B + InOther.B};
}

Color Color::Clamp() const
{
    Color Result;
    Result.R = R > 1.f ? 1.f : R;
    Result.G = G > 1.f ? 1.f : G;
    Result.B = B > 1.f ? 1.f : B;
    return Result;
}