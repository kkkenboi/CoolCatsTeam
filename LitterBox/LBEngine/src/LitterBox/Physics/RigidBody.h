/*!************************************************************************
 \file                RigidBody.h
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                04-09-2023
 \brief
 Contains the non-member functions of the CPRigidBody class. The class
 declaration of CPRigidBody and all its member functions are in Components.h

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "LitterBox/Utils/Math.h"
#include "Collisions.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
    /*!***********************************************************************
        \brief
        Creates the Vertice Box placing it in the give Vec2<float> pointer
    *************************************************************************/
    void CreateBoxVertices(LB::Vec2<float>* vertices_arr, float width, float height);

    /*!***********************************************************************
        \brief
        Creates the BoxVertices Transformed
    *************************************************************************/
    void CreateBoxVerticesTransformed(LB::Vec2<float>* vertices_arr, LB::Vec2<float> position);
}