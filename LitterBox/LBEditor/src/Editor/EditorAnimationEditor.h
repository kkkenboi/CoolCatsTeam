/*!************************************************************************
 \file				EditorAnimationEditor.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the EditorAnimationEditor class and all its functionalities
 for the animation view layer of the Editor.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Engine/Input.h"

#include "imgui_neo_sequencer.h"

#include "EditorAnimationView.h"

namespace LB 
{

	/*!***********************************************************************
	  \brief
	  This editor layer is responsible for the animation view of the editor.
	  It allows the user to load, save, and edit the animation states and controllers.
	*************************************************************************/
	class EditorAnimationEditor : public Layer, public Singleton<EditorAnimationEditor> 
	{
	public:
		/*!***********************************************************************
		  \brief
		  Sets up the default layer name.
		*************************************************************************/
		EditorAnimationEditor() : Layer("Animation Editor") {};

		/*!***********************************************************************
		  \brief
		  Sets up the layer name.
		*************************************************************************/
		EditorAnimationEditor(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Renders the animation editor layer.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Updates the loaded animation state.
		*************************************************************************/
		void UpdateLoadedState();

		/*!***********************************************************************
		  \brief
		  Updates the loaded animation controller.
		*************************************************************************/
		void UpdateLoadedController();

		/*!***********************************************************************
		  \brief
		  Loads the animation state for editing.
		*************************************************************************/
		void LoadState(std::string const& name);

		/*!***********************************************************************
		  \brief
		  Loads the animation controller for editing.
		*************************************************************************/
		void LoadController(std::string const& name);

		/*!***********************************************************************
		  \brief
		  Saves the currently loaded animation state/controller.
		*************************************************************************/
		void Save();

		/*!***********************************************************************
		  \brief
		  Destroys the animation editor layer.
		*************************************************************************/
		void Destroy() {}

		/*!***********************************************************************
		  \brief
		  Handles the deleting of animation keyframes.
		*************************************************************************/
		template <typename T>
		void CheckDeleteKeyFrame(LBKeyFrameGroup<T>& group)
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_DELETE) && ImGui::GetNeoKeyframeSelectionSize())
			{
				ImGui::FrameIndexType* keyFramesToRemove = new ImGui::FrameIndexType[ImGui::GetNeoKeyframeSelectionSize()];
				ImGui::GetNeoKeyframeSelection(keyFramesToRemove);

				for (uint32_t index{ 0 }; index < ImGui::GetNeoKeyframeSelectionSize(); ++index)
				{
					group.Remove(LBKeyFrame<T>{keyFramesToRemove[index]});
				}
			}
		}
		
	private:
		// For animation view to use private variables for previewing
		friend class EditorAnimationView;

		// For checking what is loaded
		bool m_stateLoaded{ false }, m_controllerLoaded{ false };

		//----------------------------------------------Animation State----------------------------------------------
		LBAnimationState m_loadedState{};

		// For editing state values
		bool m_editActive{};
		float m_editPosX{}, m_editPosY{}, m_editScaleX{}, m_editScaleY{}, m_editRot{};
		int m_editSprite{};

		// For timeline state
		bool m_transformOpen{ true }, m_posOpen{ true }, m_scaleOpen{ true };

		// For preview
		bool m_previewPlaying{ false };
		float m_elapsedTime{ 0.0f }, m_frameDuration{ 1.0f / 60.0f };
		ImGui::FrameIndexType m_currentFrame{ 0 }, m_startFrame{ 0 }, m_endFrame{ 60 };

		// For sprite editting
		SpriteSheet* m_spriteSheet{ nullptr };

		//----------------------------------------------Animation Controller----------------------------------------------
		AnimationController m_currentController{};
	};
}