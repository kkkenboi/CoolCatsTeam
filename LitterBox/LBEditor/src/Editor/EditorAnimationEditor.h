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

		//ImVec4 m_buttonOffColor{ 0.05f, 0.2f, 0.4f, 1.0f };
		//ImVec4 m_buttonOnColor{ 0.2f, 0.6f, 1.0f, 1.0f };

		//inline unsigned int getNumOfTiles() { return tiles.size(); }
		//inline int getTextureID() { return textureID; }
		//inline auto getMMUV(int index) { return tiles.at(index); }

	private:
		bool m_stateLoaded{ false }, m_controllerLoaded{ false };

		KeyFrame m_currentKeyFrame{};
		AnimationState m_currentState{};

		AnimationController m_currentController{};
	};
}