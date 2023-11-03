/*!************************************************************************
 \file				EditorGameView.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This header file contains functions declarations for the game view layer of the
 Editor. This is to show how the game will look like during game mode, based on 
 the player's perspective.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	class EditorGameView : public Layer
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorGameView class.
		  \return
		  Nothing.
		*************************************************************************/
		EditorGameView(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorGameView layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorGameView layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy() {}

		/*!***********************************************************************
		  \brief
		  Get the current mouse position in the game view.
		  \return
		  The mouse position.
		*************************************************************************/
		Vec2<float>& GetMousePos();

	private:
		Vec2<float> m_MousePos;
	};

	extern EditorGameView* EDITORGAMEVIEW;
}