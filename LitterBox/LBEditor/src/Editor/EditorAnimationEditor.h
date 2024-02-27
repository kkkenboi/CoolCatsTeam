/*!************************************************************************
 \file				EditorAnimationEditor.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the EditorAniamtionEditor class and all its functionalities
 for the aniamtion view layer of the Editor.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"

#include "imgui_neo_sequencer.h"

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
		
	private:
		//----------------------------------------------Animation State----------------------------------------------
		bool m_stateLoaded{ false }, m_controllerLoaded{ false }, m_previewPlaying{ false };
		int m_previewIndex{ 0 };
		float m_previewTimeElapsed{ 0.0f }, m_allDuration{ 0.0f };
		SpriteSheet* m_spriteSheet{ nullptr };

		ImGui::FrameIndexType m_startFrame{ 0 }, m_endFrame{ 64 }, m_currentFrame{ 0 };

		KeyFrame m_currentKeyFrame{}, m_tempKeyFrame{};
		AnimationState m_currentState{};

		//----------------------------------------------Animation Controller----------------------------------------------
		AnimationController m_currentController{};
	};
}