
#pragma once

#include "LitterBox/Utils/Math.h"
#include "Collisions.h"
#include "LitterBox/Factory/Components.h"

struct AABB;

namespace LB
{
  // End of RigidBody class

    void CreateBoxVertices(LB::Vec2<float>* vertices_arr, float width, float height);

    void CreateBoxVerticesTransformed(LB::Vec2<float>* vertices_arr, LB::Vec2<float> position);

}