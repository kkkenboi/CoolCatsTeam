/*!************************************************************************
 \file				EditorInspector.cpp
 \author(s)			Kenji Brannon Chong, Ang Jiawei Jarrett
 \par DP email(s):	kenjibrannon.c@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions defintions for the inspector layer of the
 Editor. This is a generic window that displays any information of the item
 selected by the user.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorInspector.h"

namespace LB
{
	EditorInspector* EDITORINSPECTOR{ nullptr };

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorInspector class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorInspector::EditorInspector(std::string layerName) : Layer(layerName)
	{
		if (!EDITORINSPECTOR)
			EDITORINSPECTOR = this;
		else
			DebuggerLogError("Editor Inspector already exist!");
	}

	/*!***********************************************************************
	  \brief
	  Initializes the EditorInspector class.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorInspector::Initialize()
	{
		InspectorGameObject::Instance()->Initialize();
		InspectorSpriteSheet::Instance()->Initialize();
	}

	/*!***********************************************************************
	 \brief
	 Updates the EditorInspector layer.
	*************************************************************************/
	void EditorInspector::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str(), 0, 0);

		if (m_currentWindow)
		{
			m_currentWindow->UpdateLayer();
		}
		//----------------------------------------INSPECT COMPONENTS WINDOW---------------------------------------

		ImGui::End();
	}

	template<typename T>
	void EditorInspector::SetWindow()
	{
		m_currentWindow = T::Instance();
	}

	void EditorInspector::SetWindowGameObject()
	{
		SetWindow<InspectorGameObject>();
	}

	void EditorInspector::SetWindowSpriteSheet()
	{
		SetWindow<InspectorSpriteSheet>();
	}

	void EditorInspector::ClearWindow()
	{
		m_currentWindow.reset();
	}
}