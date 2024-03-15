/*!************************************************************************
 \file                EditorCollisionLayer.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                25-11-2023
 \brief
 This file contains the class EditorCollisionLayer which handles
 the ImGui display of the CollisionLayerSystem of the game

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "EditorAnimationEditor.h"
#include "EditorAnimationView.h"
#include "LitterBox/Renderer/Renderer.h"

namespace LB
{
	static float smallWidth = 50.f;

	/*!***********************************************************************
	  \brief
	  This constructor initialises the pointer to the Singleton class of the
	  EditorCollisionLayer
	*************************************************************************/
	EditorAnimationView::EditorAnimationView(std::string layerName) : Layer(layerName)
	{

	}

	/*!***********************************************************************
	  \brief
	  This function initializes the EditorCollisionLayer
	*************************************************************************/
	void EditorAnimationView::Initialize()
	{
		m_editorSSheet = &ASSETMANAGER->GetSpriteSheet("EditorSpriteSheet");
	}

	/*!***********************************************************************
	  \brief
	  This function updates the ImGui window to contain the elements of
	  the CollisionLayerSystem, allowing you to display the current layer
	  and what it can collide with as well as editing the amount of layers
	  currently being used within the game scene
	*************************************************************************/
	void EditorAnimationView::UpdateLayer()
	{
		if (ImGui::Begin(GetName().c_str()))
		{
			std::shared_ptr<EditorAnimationEditor> animEditor = EditorAnimationEditor::Instance();
			if (!animEditor->m_stateLoaded)
			{
				ImGui::End();
				return;
			}
			ImGui::Text("If no sprite is selected, despair will be shown");

			ImVec2 m_windowSize = ImGui::GetWindowSize();
			/*uint64_t warning_remover = (uint64_t)Renderer::GRAPHICS->get_scene_view();
			if (warning_remover != static_cast<unsigned int>(-1))
				ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(Renderer::GRAPHICS->get_scene_view())), m_windowSize, ImVec2(0, 1), ImVec2(1, 0));*/

			if (!animEditor->m_loadedState.m_active.GetCurrentExact(animEditor->m_currentFrame))
			{
				ImGui::End();
				return;
			}
			ImVec2 pos{ m_windowSize.x * 0.5f + animEditor->m_loadedState.m_pos.GetCurrentExact(animEditor->m_currentFrame).x
				, m_windowSize.y * 0.5f - animEditor->m_loadedState.m_pos.GetCurrentExact(animEditor->m_currentFrame).y };

			Vec2<float> scale = animEditor->m_loadedState.m_scale.GetCurrentExact(animEditor->m_currentFrame);
			pos.x -= scale.x * smallWidth * 0.5f;

			ImGui::SetCursorPos(pos);
			if (animEditor->m_spriteSheet)
			{
				int spriteIndex = animEditor->m_loadedState.m_sprite.GetCurrentExact(animEditor->m_currentFrame);

				ImGui::Image((ImTextureID)(uint64_t)ASSETMANAGER->GetTextureIndex(animEditor->m_spriteSheet->GetPNGRef()), ImVec2{ smallWidth * scale.x, smallWidth * scale.y }
					, ImVec2{ (*animEditor->m_spriteSheet)[spriteIndex].m_min.x, (*animEditor->m_spriteSheet)[spriteIndex].m_max.y }
				, ImVec2{ (*animEditor->m_spriteSheet)[spriteIndex].m_max.x, (*animEditor->m_spriteSheet)[spriteIndex].m_min.y });
			}
			else
			{
				ImGui::Image((ImTextureID)(uint64_t)ASSETMANAGER->GetTextureIndex(m_editorSSheet->GetPNGRef()), ImVec2{ smallWidth * scale.x , smallWidth * scale.y }
					, ImVec2{ (*m_editorSSheet)[5].m_min.x, (*m_editorSSheet)[5].m_max.y }
					, ImVec2{ (*m_editorSSheet)[5].m_max.x, (*m_editorSSheet)[5].m_min.y });
			}

		}
		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  This function is called when EditorCollisionLayer is destroyed
	*************************************************************************/
	void EditorAnimationView::Destroy()
	{

	}
}