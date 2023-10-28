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
#include "LitterBox/Scene/SceneManager.h"

namespace LB
{
    class EditorHierarchy : public Layer
    {
    public:
        EditorHierarchy(std::string layerName);

        void Initialize() override;

        void UpdateLayer() override;

        void DrawRoot();
        bool DrawItem(CPTransform* item);

        void UpdateSceneLoaded(Scene* loadedScene);

        Event<GameObject*> onNewObjectSelected;

    private:
        CPTransform* m_clickedItem{ nullptr };
        Scene* m_loadedScene{ nullptr };
    };

    // For Event Subscription
    void UpdateSceneLoaded(Scene* loadedScene);

    extern EditorHierarchy* EDITORHIERACHY;
}
