/*!************************************************************************
 \file				InspectorGameObject.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"

#include "LitterBox/Core/Singleton.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	class InspectorGameObject : public Layer, public Singleton<InspectorGameObject>
	{
    public:
        /*!***********************************************************************
          \brief

        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief

        *************************************************************************/
        void UpdateLayer() override;

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

        bool isPrefab = false;
    private:
        GameObject* m_inspectedGO{ nullptr };
        char m_inspectedName[256]{}, m_textBuffer[256]{};

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
    *************************************************************************/
    void UpdateInspectedGO(GameObject* newInspectedGO);

    /*!***********************************************************************
      \brief
      To get the original function, DeselectObject called as an event.
    *************************************************************************/
    void DeselectObject(bool isPlaying);

    /*!***********************************************************************
      \brief
      To get the original function, DeselectObject called as an event, overloaded.
    *************************************************************************/
    void DeselectObject(Scene* newScene);

}