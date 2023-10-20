/*!************************************************************************
 \file                PhysicsMath.h
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                04-09-2023
 \brief
 This file contains the function declarations for maths needed for physics

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "RigidBody.h"
#include <cmath>
#include "LitterBox/Utils/Math.h"


namespace PHY_MATH {
    /*!***********************************************************************
      \brief
      Clamps the value to either the min or max given
      \return
      Returns the clamped value as a float
    *************************************************************************/
    float Clamp(float value, float min, float max);

    /*!***********************************************************************
      \brief
      Clamps the value to either the min or max given
      \return
      Returns the clamped value as a float
    *************************************************************************/
    float Length(LB::Vec2<float> vec);

    /*!***********************************************************************
      \brief
      Gets the distance between two vectors
      \return
      Returns the distance between the two vectors as a float
    *************************************************************************/
    float Distance(LB::Vec2<float> vecA, LB::Vec2<float> vecB);

    /*!***********************************************************************
      \brief
      Normalizes the given vector
      \return
      Returns the given vector as a normalized vector
    *************************************************************************/
    LB::Vec2<float> Normalize(LB::Vec2<float> vec);
    
    /*!***********************************************************************
      \brief
      Dot Products two vectors together
      \return
      Returns the Dot Product of two vectors together as a float value
    *************************************************************************/
    float DotProduct(LB::Vec2<float> vecA, LB::Vec2<float> vecB);

    /*!***********************************************************************
      \brief
      Cross Products two vectors together
      \return
      Returns the Cross Product of two vectors together as a float value
    *************************************************************************/
    float CrossProduct(LB::Vec2<float> vecA, LB::Vec2<float> vecB);

    /*!***********************************************************************
      \brief
      Finds the minimum of the two given values
      \returns
      Returns the lowest number out of the two values
    *************************************************************************/
    float FindMin(float valA, float valB);

    float Absolute(float val);
}