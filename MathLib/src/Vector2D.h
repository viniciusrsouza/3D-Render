#pragma once

#ifdef MATHLIB_EXPORTS
#define VECTOR_2D_API __declspec(dllexport)
#else
#define VECTOR_2D_API __declspec(dllimport)
#endif

#include <iostream>

namespace MathLib {
    class VECTOR_2D_API Vector2D {
    public:
        float x, y;

        /**
         * Default constructor, initialize x and y with 0.
         */
        Vector2D();

        /**
         * Copy constructor, load the values of x, y and z from
         * the vector u to the new instance.
         * @param u: source vector
         */
        Vector2D(const Vector2D& u) = default;

        /**
         * Explicit constructor, initialize the vector with the
         * provided x, y and z values.
         * @param nX: initial x.
         * @param nY: initial y.
         * @param nZ: initial z.
         */
        Vector2D(float nX, float nY);

        /**
         * assigns a vector to another by reassigning
         * the x, y and z values.
         * @param u: source vector.
         * @return [this] after reassignment.
         */
        Vector2D& operator=(const Vector2D& u);

        /**
         * asserts equality of vectors by asserting
         * equality of both vectors' x, y and z.
         * @param u: vector to be compared.
         * @return
         */
        bool operator==(const Vector2D& u) const;

        /**
         * Negate a vector by negating all of its values.
         * @return [this] negated.
         */
        Vector2D operator-() const;

        /**
         * transform a vector into a zero vector.
         */
        void zero();

        /**
         * Binary minus, subtracts two vectors.
         * @return [this] - [u]
         */
        Vector2D operator-(const Vector2D& u) const;

        /**
         * Binary plus, adds two vectors.
         * @return [this] + [u]
         */
        Vector2D operator+(const Vector2D& u) const;

        /**
         * Binary scalar multiplication,
         * multiplies a vector by a scalar.
         * @return [this] * [c]
         */
        Vector2D operator*(float c) const;

        /**
         * Binary scalar division,
         * divides a vector by a scalar.
         * @return [this] / [c]
         */
        Vector2D operator/(float c) const;

        /**
         * Add-assignment, assigns to [this]
         * [this] + [u].
         * @return [this]
         */
        Vector2D operator+=(const Vector2D& u);

        /**
         * Minus-assignment, assigns to [this]
         * [this] - [u].
         * @return [this]
         */
        Vector2D operator-=(const Vector2D& u);

        /**
         * Scalar multiplication assignment, assigns to [this]
         * [this] * [c].
         * @return [this]
         */
        Vector2D operator*=(float c);

        /**
         * Scalar division assignment, assigns to [this]
         * [this] / [c].
         * @return [this]
         */
        Vector2D operator/=(float c);

        /**
         * @return Dot product between [this] and [u].
         */
        float dot(const Vector2D& u) const;

        /**
         * @return Cross product between [this] and [u].
         */
        Vector2D cross(const Vector2D& u) const;

        /**
         * @return Magnitude (length) of [this].
         */
        float magnitude() const;

        /**
         * @return a normalized vector of [this].
         */
        Vector2D normalize() const;

        /**
         * @return distance of [this] (this / ||this||).
         */
        float distance(const Vector2D& u) const;

        std::string str() const;
    };

    /**
     * Scalar multiplication operator to allow left scalar
     * notation.
     */
    VECTOR_2D_API inline Vector2D operator*(float c, Vector2D& u);

    /**
     * toString operator override
     */
    VECTOR_2D_API inline std::ostream& operator<<(std::ostream& Str, Vector2D& v);

    // Const zero vector.
    VECTOR_2D_API const Vector2D ZeroVector;
}