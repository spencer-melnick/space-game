#include "vector2d.h"

using namespace Game;

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
    x += vec.x;
    y += vec.y;
    return *(this);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    x -= vec.x;
    y -= vec.y;
    return *(this);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
    x *= vec.x;
    y *= vec.y;
    return *(this);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
    x /= vec.x;
    y /= vec.y;
    return *(this);
}

Vector2D& Vector2D::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    return *(this);
}

Vector2D& Vector2D::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    return *(this);
}

Vector2D Vector2D::operator+(const Vector2D& vec) const
{
    return (Vector2D(*this) += vec);
}

Vector2D Vector2D::operator-(const Vector2D& vec) const
{
    return (Vector2D(*this) -= vec);
}

Vector2D Vector2D::operator*(const Vector2D& vec) const
{
    return (Vector2D(*this) *= vec);
}

Vector2D Vector2D::operator/(const Vector2D& vec) const
{
    return (Vector2D(*this) /= vec);
}

Vector2D Vector2D::operator*(const float scalar) const
{
    return (Vector2D(*this) *= scalar);
}

Vector2D Vector2D::operator/(const float scalar) const
{
    return (Vector2D(*this) /= scalar);
}

float Vector2D::magnitude() const
{
    return sqrtf(x * x + y * y);
}

Vector2D& Vector2D::normalize()
{
    return ((*this) /= magnitude());
}

Vector2D Vector2D::normalized() const
{
    return (Vector2D(*this).normalize());
}
