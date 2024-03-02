/*!************************************************************************
 \file				EditorSceneView.h
 \author(s)			Ang Jiawei Jarrett, Kenji Brannon Chong
 \par DP email(s):	a.jiaweijarrett@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file contains functions declarations for the scene view layer of the
 Editor. This is to showcase the scene view, apart from the game view where it
 can be interacted with and moved around.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include <ImGuizmo.h>


namespace LB
{
	class EditorSceneView : public Layer
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorSceneView class.
		  \return
		  Nothing.
		*************************************************************************/
		EditorSceneView(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Initializes the EditorSceneView layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		  \brief
		  Updates the EditorSceneView layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		//void Destroy() override;

		/*!***********************************************************************
		  \brief
		  Set the selected object in the scene.
		  \return
		  Nothing.
		*************************************************************************/
		void SetObjectPicked(GameObject* obj);

		/*!***********************************************************************
		  \brief
		  Moves the scene view camera upwards.
		  \return
		  Nothing.
		*************************************************************************/
		void MoveCamUp(float dist = 20.0f);

		/*!***********************************************************************
		  \brief
		  Moves the scene view camera downwards.
		  \return
		  Nothing.
		*************************************************************************/
		void MoveCamDown(float dist = 20.0f);

		/*!***********************************************************************
		  \brief
		  Moves the scene view camera leftwards.
		  \return
		  Nothing.
		*************************************************************************/
		void MoveCamLeft(float dist = 20.0f);

		/*!***********************************************************************
		  \brief
		  Moves the scene view camera rightwards.
		  \return
		  Nothing.
		*************************************************************************/
		void MoveCamRight(float dist = 20.0f);

		/*!***********************************************************************
		  \brief
		  Moves the scene view camera.
		*************************************************************************/
		void MoveCam(Vec2<float> const& dist);

		/*!***********************************************************************
		  \brief
		  Zooms the scene view camera inwards.
		  \return
		  Nothing.
		*************************************************************************/
		void ZoomCamIn();

		/*!***********************************************************************
		  \brief
		  Zooms the scene view camera outwards.
		  \return
		  Nothing.
		*************************************************************************/
		void ZoomCamOut();

	private:
		ImVec2 m_windowSize;
		bool usingSceneView{ false };

		float m_camDragSpeed{ 50.0f };
		KeyCode m_moveCamKey{ KeyCode::KEY_LEFTALT };

		// Mouse picker to select GameObjects
		GameObject* m_mousePicker{ nullptr };
		Vec2<float> m_mousePosInWorld;

		Vec2<float> m_oldMousePosInWorld; // For internal use, caching
	};

	extern EditorSceneView* EDITORSCENEVIEW;
}
