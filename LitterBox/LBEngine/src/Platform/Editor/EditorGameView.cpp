/*!************************************************************************
 \file				EditorGameView.cpp
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorGameView.h"
#include "LitterBox/Renderer/Renderer.h"

namespace LB
{
	EditorGameView::EditorGameView(std::string layerName) : Layer(layerName)
	{

	}

	void EditorGameView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// TO DO: The aspect ratio bar is at the top

		ImVec2 currWinPos = ImGui::GetWindowPos();
		ImVec2 currWinSize = ImGui::GetContentRegionAvail();

		// If winPos and winPos has changed, update renderer to display the new viewport
		if (m_winPos.x != currWinPos.x || m_winPos.y != currWinPos.y ||
			m_winSize.x != currWinSize.x || m_winSize.y != currWinSize.y)
		{
			// Update the pos and size to the new one
			m_winPos = currWinPos;
			m_winSize = currWinSize;

			Renderer::GRAPHICS->UpdateGameWindowPos((GLint)m_winPos.x, (GLint)m_winPos.y, (GLsizei)m_winSize.x, (GLsizei)m_winSize.y);
		}

		ImGui::End();
	}
}
