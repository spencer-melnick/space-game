#pragma once

#include <math.h>

namespace Game
{
    class Vector2D
    {
        public:
            float x = 0;
            float y = 0;

            Vector2D& operator+=(const Vector2D& vec);
            Vector2D& operator-=(const Vector2D& vec);
            Vector2D& operator*=(const Vector2D& vec);
            Vector2D& operator/=(const Vector2D& vec);
            Vector2D& operator*=(const float scalar);
            Vector2D& operator/=(const float scalar);

            Vector2D operator+(const Vector2D& vec) const;
            Vector2D operator-(const Vector2D& vec) const;
            Vector2D operator*(const Vector2D& vec) const;
            Vector2D operator/(const Vector2D& vec) const;
            Vector2D operator*(const float scalar) const;
            Vector2D operator/(const float scalar) const;

            float magnitude() const;
            Vector2D& normalize();
            Vector2D normalized() const;
    };
}
