/*!************************************************************************
 \file				EditorToolBar.h
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file contains functions declarations for the toolbar layer of the
 Editor. This is to allow the programmer to play, stop and step in frame by frame.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	class EditorToolBar : public Layer
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorToolBar class.
		  \return
		  Nothing.
		*************************************************************************/
		EditorToolBar(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorToolBar layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorToolBar layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy() {}

	private:
		float m_buttonSpacing { 12.f };
		ImVec2 m_buttonSize{ 24.f, 24.f };

		ImVec4 m_buttonOffColor{ 0.05f, 0.2f, 0.4f, 1.0f };
		ImVec4 m_buttonOnColor{ 0.2f, 0.6f, 1.0f, 1.0f };
	};
}
