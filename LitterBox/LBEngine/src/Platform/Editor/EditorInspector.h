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
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
    class EditorInspector : public Layer
    {
    public:
        EditorInspector(std::string layerName);

        void Initialize() override;

        void UpdateLayer() override;

        void Destroy() {}

        void UpdateInspectedGO(GameObject* newInspectedGO);

    private:
        GameObject* m_inspectedGO{ nullptr };
        char m_inspectedName[256]{};
    };
    
    // For event subscription
    void UpdateInspectedGO(GameObject* newInspectedGO);
    void DeselectObject(bool isPlaying);

    extern EditorInspector* EDITORINSPECTOR;
}

