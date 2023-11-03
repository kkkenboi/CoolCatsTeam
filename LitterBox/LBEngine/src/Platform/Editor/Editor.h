/*!************************************************************************
 \file				Editor.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This file contains functions declarations for the Editor where it manages
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

		/*!***********************************************************************
		  \brief
		  Constructor for the Editor class.
		  \return
		  Nothing.
		*************************************************************************/
		Editor();

		/*!***********************************************************************
		  \brief
		  Initializes the Editor system.
		  \return
		  Nothing.
		*************************************************************************/
		void Initialize()	override;

		/*!***********************************************************************
		  \brief
		  Updates the Editor system.
		  \return
		  Nothing.
		*************************************************************************/
		void Update()		override;

		/*!***********************************************************************
		  \brief
		  Destroys the Editor system.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy()		override;

	private:
		LayerStack m_ImGuiLayers;
	};

	extern Editor* EDITOR;
}
