#include "Math.h"

namespace PHY_MATH {

    float Clamp(float value, float min, float max) {

        if (value < min) {
            return min;
        }

        if (value > max) {
            return max;
        }

        return value;
    }

    // Get the length of a vector
    float Length(Vec2<float> vec) {
        return sqrt(vec.x * vec.x + vec.y + vec.y);
    }

    // Get distance from vector position A and vector position B
    float Distance(Vec2<float> vecA, Vec2<float> vecB) {
        float dx = vecA.x - vecB.x;
        float dy = vecA.y - vecB.y;
        return sqrt(dx * dx + dy * dy);
    }

    // Get only the direction vector of the given vector multiplied by magnitude of 1
    Vec2<float> Normalize(Vec2<float> vec) {
        float len = Length(vec);
        Vec2<float> norm_vec;
        norm_vec.x = vec.x / len;
        norm_vec.y = vec.y / len;
        return norm_vec;
    }

    float DotProduct(Vec2<float> vecA, Vec2<float> vecB) {
        // a . b = ax x bx + ay x by
        return (vecA.x * vecB.x + vecA.y * vecB.y);
    }

    float CrossProduct(Vec2<float> vecA, Vec2<float> vecB) {
        // cz = ax x by - ay x bx
        return vecA.x * vecB.y - vecA.y * vecB.x;
    }

    float FindMin(float valA, float valB) {
        if (valA < valB) {
            return valA;
        }
        return valB;
    }

}