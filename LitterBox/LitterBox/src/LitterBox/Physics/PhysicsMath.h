
#pragma once

#include "RigidBody.h"
#include <cmath>


namespace PHY_MATH {
    float Clamp(float value, float min, float max);

    // Get the length of a vector
    float Length(Vec2<float> vec);

    // Get distance from vector position A and vector position B
    float Distance(Vec2<float> vecA, Vec2<float> vecB);

    // Get only the direction vector of the given vector multiplied by magnitude of 1
    Vec2<float> Normalize(Vec2<float> vec);
    
    float DotProduct(Vec2<float> vecA, Vec2<float> vecB);

    float CrossProduct(Vec2<float> vecA, Vec2<float> vecB);

    float FindMin(float valA, float valB);
}