#pragma once

#include <math.h>

namespace Game
{
    template <typename T>
    class Vector2D
    {
        public:
            T x = 0;
            T y = 0;

            Vector2D& operator+=(const Vector2D& vec);
            Vector2D& operator-=(const Vector2D& vec);
            Vector2D& operator*=(const Vector2D& vec);
            Vector2D& operator/=(const Vector2D& vec);
            Vector2D& operator*=(const T scalar);
            Vector2D& operator/=(const T scalar);

            Vector2D operator+(const Vector2D& vec) const;
            Vector2D operator-(const Vector2D& vec) const;
            Vector2D operator*(const Vector2D& vec) const;
            Vector2D operator/(const Vector2D& vec) const;
            Vector2D operator*(const T scalar) const;
            Vector2D operator/(const T scalar) const;

            T magnitude() const;
            Vector2D& normalize();
            Vector2D normalized() const;
    };


    template <typename T>
    Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& vec)
    {
        x += vec.x;
        y += vec.y;
        return *(this);
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& vec)
    {
        x -= vec.x;
        y -= vec.y;
        return *(this);
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::operator*=(const Vector2D<T>& vec)
    {
        x *= vec.x;
        y *= vec.y;
        return *(this);
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::operator/=(const Vector2D<T>& vec)
    {
        x /= vec.x;
        y /= vec.y;
        return *(this);
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::operator*=(const T scalar)
    {
        x *= scalar;
        y *= scalar;
        return *(this);
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::operator/=(const T scalar)
    {
        x /= scalar;
        y /= scalar;
        return *(this);
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& vec) const
    {
        return (Vector2D<T>(*this) += vec);
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& vec) const
    {
        return (Vector2D<T>(*this) -= vec);
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::operator*(const Vector2D<T>& vec) const
    {
        return (Vector2D<T>(*this) *= vec);
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::operator/(const Vector2D<T>& vec) const
    {
        return (Vector2D<T>(*this) /= vec);
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::operator*(const T scalar) const
    {
        return (Vector2D<T>(*this) *= scalar);
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::operator/(const T scalar) const
    {
        return (Vector2D<T>(*this) /= scalar);
    }

    template <typename T>
    T Vector2D<T>::magnitude() const
    {
        return sqrtf(x * x + y * y);
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::normalize()
    {
        return ((*this) /= magnitude());
    }

    template <typename T>
    Vector2D<T> Vector2D<T>::normalized() const
    {
        return (Vector2D<T>(*this).normalize());
    }

}
