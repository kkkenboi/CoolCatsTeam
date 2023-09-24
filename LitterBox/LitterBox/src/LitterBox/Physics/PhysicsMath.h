
#pragma once

#include "RigidBody.h"
#include <cmath>
#include "LitterBox/Utils/Math.h"


namespace PHY_MATH {
    float Clamp(float value, float min, float max);

    // Get the length of a vector
    float Length(LB::Vec2<float> vec);

    // Get distance from vector position A and vector position B
    float Distance(LB::Vec2<float> vecA, LB::Vec2<float> vecB);

    // Get only the direction vector of the given vector multiplied by magnitude of 1
    LB::Vec2<float> Normalize(LB::Vec2<float> vec);
    
    float DotProduct(LB::Vec2<float> vecA, LB::Vec2<float> vecB);

    float CrossProduct(LB::Vec2<float> vecA, LB::Vec2<float> vecB);

    float FindMin(float valA, float valB);
}