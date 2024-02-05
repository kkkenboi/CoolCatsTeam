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
		////textureID is for actually drawing the texture in ImGui
		//		//slotID is just so the name prints properly in the ImGui dropdown textbox
		//int textureID, slotID;

		////these values are for displaying the texture properly in the editor inspector
		//float textureAspect;
		//ImVec2 textureSize;

		////store the min max UV for each tile respectively


		//std::vector< //tiles
		//	std::pair< //uv
		//	std::pair<float, float>,  //min x,y
		//	std::pair<float, float> //max x,y
		//	>
		//> tiles;

		////storing the animation frames into a vector, so later it can be played
		////std::vector<> frames;

		////void SpilttingTheSprites();

		//void SettingAnimation();

	public:
		EditorAnimationEditor(std::string layerName);

		void UpdateLayer() override;

		void Destroy() {}

		//ImVec4 m_buttonOffColor{ 0.05f, 0.2f, 0.4f, 1.0f };
		//ImVec4 m_buttonOnColor{ 0.2f, 0.6f, 1.0f, 1.0f };

		//inline unsigned int getNumOfTiles() { return tiles.size(); }
		//inline int getTextureID() { return textureID; }
		//inline auto getMMUV(int index) { return tiles.at(index); }

	private:

	};
}