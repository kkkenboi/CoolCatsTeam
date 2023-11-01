/*!************************************************************************
 \file				Editor.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions declartions for the Editor where it manages
 all of the different layers of the editor, eg. the Hierarchy layer, the
 Inspector, and more. It ensures that all the layers are updated and
 rendered in order.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/LayerStack.h"
#include "LitterBox/Factory/GameObjectManager.h"

namespace LB
{
	class Editor : public ISystem
	{
	public:
		Editor();

		void Initialize()	override;
		void Update()		override;
		void FixedUpdate()	override;
		void Destroy()		override;

		GameObject* GetMousePicker();
		void SetObjectPicked(GameObject* obj);
		void SetMousePos(Vec2<float> pos);

	private:
		LayerStack m_ImGuiLayers;
		
		GameObject* m_MousePicker;
	};

	extern Editor* EDITOR;
}
