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

namespace LB 
{
	class EditorAnimationEditor : public Layer, public Singleton<EditorAnimationEditor> 
	{
	public:
		EditorAnimationEditor() : Layer("Animation Editor") {};

		EditorAnimationEditor(std::string layerName);

		void UpdateLayer() override;

		void LoadState(std::string const& name);

		void LoadController(std::string const& name);

		void Save();

		void Destroy() {}
		
	private:
		bool m_stateLoaded{ false }, m_controllerLoaded{ false };

		KeyFrame m_currentKeyFrame{}, m_tempKeyFrame{};
		AnimationState m_currentState{};

		AnimationController m_currentController{};
	};
}