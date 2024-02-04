/*!************************************************************************
 \file				EditorHierarchy.h
 \author(s)			Kenji Brannon Chong, Ang Jiawei Jarrett
 \par DP email(s):	kenjibrannon.c@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions defintions for the inspector layer of the
 Editor. This is a generic window that displays any information of the item
 selected by the user.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"

#include "Inspector/InspectorGameObject.h"
#include "Inspector/InspectorSpriteSheet.h"

#include <ImGuizmo.h>

namespace LB
{
    class EditorInspector : public Layer
    {
    public:
        /*!***********************************************************************
          \brief
          Constructor for the EditorInspector class.
          \return
          Nothing.
        *************************************************************************/
        EditorInspector(std::string layerName);

        /*!***********************************************************************
          \brief
          Initializes the EditorInspector layer.
        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief
          Updates the EditorInspector layer.
        *************************************************************************/
        void UpdateLayer() override;

        /*!***********************************************************************
          \brief
          Destroys the EditorInspector layer.
        *************************************************************************/
        void Destroy() {}

        template<typename T>
        void SetWindow();

        void SetWindowGameObject();

        void SetWindowSpriteSheet();

        void ClearWindow();

    private:
        std::shared_ptr<Layer> m_currentWindow;
    };
    
    extern EditorInspector* EDITORINSPECTOR;
}

