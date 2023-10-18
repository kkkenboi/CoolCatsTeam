/*!************************************************************************
 \file				EditorHierarchy.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions declarations for the hierarchy layer of the
 Editor. This is to ensure that there is a overview of all the game objects
 in the game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
    class EditorInspector : public Layer
    {
    public:
        EditorInspector(std::string layerName);

        void Initialize() override;

        void UpdateLayer() override;
    };
}

