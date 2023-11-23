/*!************************************************************************
 \file				EditorHierarchy.h
 \author(s)			Kenji Brannon Chong, Vanessa Chua Siew Jin
 \par DP email(s):	kenjibrannon.c@digipen.edu, vanessasiewjin.chua@digipen.edu
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

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
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
          \return
          Nothing.
        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief
          Updates the EditorInspector layer.
          \return
          Nothing.
        *************************************************************************/
        void UpdateLayer() override;

        /*!***********************************************************************
          \brief
          Destroys the EditorInspector layer.
          \return
          Nothing.
        *************************************************************************/
        void Destroy() {}

        /*!***********************************************************************
          \brief
          Update the currently inspected game object.
          \return
          Nothing.
        *************************************************************************/
        void UpdateInspectedGO(GameObject* newInspectedGO);

        /*!***********************************************************************
          \brief
          Get the currently inspected game object.
          \return
          A pointer to the currently inspected GameObject.
        *************************************************************************/
        GameObject* GetInspectedGO();

        /*!***********************************************************************
          \brief
          Check if a game object is currently being inspected.
          \return
          True if a game object is being inspected, false otherwise.
        *************************************************************************/
        bool IsGOInspected();
        bool isPrefab = false;

        /*!***********************************************************************
          \brief
          Get the current gizmos mode.
          \return
          Nothing.
        *************************************************************************/
        ImGuizmo::MODE GetGizmosMode() const;

        /*!***********************************************************************
          \brief
          Sets the current gizmos mode.
          \return
          Nothing.
        *************************************************************************/
        void SetGizmosMode(ImGuizmo::MODE mode);

        /*!***********************************************************************
          \brief
          Get the current gizmos operation.
          \return
          Nothing.
        *************************************************************************/
        ImGuizmo::OPERATION GetGizmosOperation() const;

        /*!***********************************************************************
          \brief
          Set the current gizmos operation.
          \return
          Nothing.
        *************************************************************************/
        void SetGizmosOperation(ImGuizmo::OPERATION operation);

        /*!***********************************************************************
          \brief
          Get the snap mode.
          \return
          Nothing.
        *************************************************************************/
        bool const& GetSnapMode() const;

        /*!***********************************************************************
          \brief
          Toggles snap mode.
          \return
          Nothing.
        *************************************************************************/
        bool& ToggleSnapMode();

        /*!***********************************************************************
          \brief
          Get the snap value of translate.
          \return
          Nothing.
        *************************************************************************/
        float const& GetSnapTranslate() const;

        /*!***********************************************************************
          \brief
          Get the snap value of rotate.
          \return
          Nothing.
        *************************************************************************/
        float const& GetSnapRotate() const;

        /*!***********************************************************************
          \brief
          Get the snap value of scale.
          \return
          Nothing.
        *************************************************************************/
        float const& GetSnapScale() const;

        /*!***********************************************************************
          \brief
          Set the snap value of translate through ImGui.
          \return
          Nothing.
        *************************************************************************/
        float& SetSnapTranslate();

        /*!***********************************************************************
          \brief
          Set the snap value of rotate through ImGui.
          \return
          Nothing.
        *************************************************************************/
        float& SetSnapRotate();

        /*!***********************************************************************
          \brief
          Set the snap value of scale through ImGui.
          \return
          Nothing.
        *************************************************************************/
        float& SetSnapScale();

    private:
        GameObject* m_inspectedGO{ nullptr };
        char m_inspectedName[256]{}, m_nameBuffer1[64]{}, m_textBuffer[256]{};

        // Manipulate Variables
        float m_SnapTranslate[2]{};
        float m_SnapRotate{};
        float m_SnapScale[2]{};

        ImGuizmo::MODE m_CurrentGizmosMode{};
        ImGuizmo::OPERATION m_CurrentGizmosOperation{};
        bool m_SnapMode{ false };
    };
    
    // For event subscription
    /*!***********************************************************************
      \brief
      To get the original function, UpdateInspectedGO called as an event.
      \return
      Nothing.
    *************************************************************************/
    void UpdateInspectedGO(GameObject* newInspectedGO);

    /*!***********************************************************************
      \brief
      To get the original function, DeselectObject called as an event.
      \return
      Nothing.
    *************************************************************************/
    void DeselectObject(bool isPlaying);

    /*!***********************************************************************
      \brief
      To get the original function, DeselectObject called as an event, overloaded.
      \return
      Nothing.
    *************************************************************************/
    void DeselectObject(Scene* newScene);

    extern EditorInspector* EDITORINSPECTOR;
}

