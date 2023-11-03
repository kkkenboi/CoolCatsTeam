/*!************************************************************************
 \file                PhysicsMath.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                04-09-2023
 \brief
 This file contains the math functions needed for physics

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Litterbox/Utils/Math.h"

namespace PHY_MATH {

    /*!***********************************************************************
      \brief
      Clamps the value to either the min or max given
      \return
      Returns the clamped value as a float
    *************************************************************************/
    float Clamp(float value, float min, float max) {

        if (value < min) {
            return min;
        }

        if (value > max) {
            return max;
        }

        return value;
    }

    /*!***********************************************************************
      \brief
      Gets the length of the given vector
      \return
      Returns the length of the vector as a float
    *************************************************************************/
    float Length(LB::Vec2<float> vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    /*!***********************************************************************
      \brief
      Gets the distance between two vectors
      \return
      Returns the distance between the two vectors as a float
    *************************************************************************/
    float Distance(LB::Vec2<float> vecA, LB::Vec2<float> vecB) {
        float dx = vecA.x - vecB.x;
        float dy = vecA.y - vecB.y;
        return sqrt(dx * dx + dy * dy);
    }

    /*!***********************************************************************
      \brief
      Normalizes the given vector
      \return
      Returns the given vector as a normalized vector
    *************************************************************************/
    LB::Vec2<float> Normalize(LB::Vec2<float> vec) {
        float len = Length(vec);
        LB::Vec2<float> norm_vec;
        norm_vec.x = vec.x / len;
        norm_vec.y = vec.y / len;
        return norm_vec;
    }

    /*!***********************************************************************
      \brief
      Dot Products two vectors together
      \return
      Returns the Dot Product of two vectors together as a float value
    *************************************************************************/
    float DotProduct(LB::Vec2<float> vecA, LB::Vec2<float> vecB) {
        // a . b = ax x bx + ay x by
        return (vecA.x * vecB.x + vecA.y * vecB.y);
    }

    /*!***********************************************************************
      \brief
      Cross Products two vectors together
      \return
      Returns the Cross Product of two vectors together as a float value
    *************************************************************************/
    float CrossProduct(LB::Vec2<float> vecA, LB::Vec2<float> vecB) {
        // cz = ax x by - ay x bx
        return vecA.x * vecB.y - vecA.y * vecB.x;
    }

    /*!***********************************************************************
      \brief
      Finds the minimum of the two given values
      \returns
      Returns the lowest number out of the two values
    *************************************************************************/
    float FindMin(float valA, float valB) {
        if (valA < valB) {
            return valA;
        }
        return valB;
    }

    /*!***********************************************************************
      \brief
      Returns the absolute value of val
      \returns
      Returns absolute value of val
    *************************************************************************/
    float Absolute(float val) 
    {
        if (val < 0)
        {
            val = -val;
        }
        return val;
    }
}