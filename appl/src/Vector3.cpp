#include "Vector3.h"
#include <cmath>

 Vector3::Vector3() 
    : X(0), Y(0), Z(0) { }


Vector3::Vector3(float InX, float InY, float InZ) 
    : X(InX), Y(InY), Z(InZ) { }

Vector3 Vector3::operator-(const Vector3& InOther) const
{
    return Vector3{X - InOther.X, Y - InOther.Y, Z - InOther.Z};
}

Vector3 Vector3::operator+(const Vector3& InOther) const
{
    return Vector3{X + InOther.X, Y + InOther.Y, Z + InOther.Z};
}

Vector3 Vector3::operator/(const float InScalar) const
{
    return Vector3{X / InScalar, Y / InScalar, Z / InScalar};
}

Vector3 Vector3::operator*(const float InScalar) const
{
    return Vector3{X * InScalar, Y * InScalar, Z * InScalar};
}


Vector3 Vector3::Normalized() const 
{
    return *this / Magnitude();
}

float Vector3::Magnitude() const 
{
    return sqrtf(MagnitudeSquared());
}

float Vector3::MagnitudeSquared() const 
{
    return Dot(*this);
}

float Vector3::Dot(const Vector3& InOther) const
{
    return X * InOther.X + Y * InOther.Y + Z * InOther.Z;
}

Vector3 Vector3::Reflect(const Vector3& InNorm) const 
{
    //R = I – 2 * dot(I,N) * N
    return *this - InNorm * (2.f * this->Dot(InNorm));
}