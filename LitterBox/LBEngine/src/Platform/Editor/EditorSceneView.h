/*!************************************************************************
 \file				EditorSceneView.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	class EditorSceneView : public Layer
	{
	public:
		EditorSceneView(std::string layerName);

		void Initialize() override;

		void UpdateLayer() override;

		void Destroy() {}

		// Updates the rest of the editor of the object selected
		void SetObjectPicked(GameObject* obj);

	private:
		ImVec2 m_windowSize;

		// Mouse picker to select GameObjects
		GameObject* m_mousePicker;
		Vec2<float> m_mousePosInWorld;
	};

	extern EditorSceneView* EDITORSCENEVIEW;
}
