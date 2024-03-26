/*!************************************************************************
 \file				Application.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 
 This file contains functions declarations of the Application class that 
 sets up the engine functionalities. This is to allow it to be used on 
 other projects.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "Platform/Windows/Windows.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/GameLogic/GameLogic.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Layer.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Debugging/Profiler.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Physics/RigidBodyManager.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Animation/AnimationManager.h"
#include "LitterBox/Animation/ParticleSystem.h"
#include "LitterBox/Components/RenderVideoComponent.h"
#include "Core.h"

namespace LB {

	/*!***********************************************************************
	 \brief
	 Sets up an application used on other projects with the LitterBox Engine
	*************************************************************************/
	class Application
	{
	public:
		/*!***********************************************************************
		 \brief
		 Constructs the LitterBox Engine with all the systems added
		*************************************************************************/
		Application(bool editorMode);

		/*!***********************************************************************
		 \brief
		 Constructs the LitterBox Engine with all the systems added
		*************************************************************************/
		void Initialise(bool startPlaying = false);

		/*!***********************************************************************
		 \brief
		 Checks if the application is running
		*************************************************************************/
		bool IsRunning();

		/*!***********************************************************************
		 \brief
		 Destroys the systems and the LitterBox Engine itself
		*************************************************************************/
		virtual ~Application();

		/*!***********************************************************************
		 \brief
		 Keeps the LitterBox Engine game loop running if program is not closing
		*************************************************************************/
		void Run();

		/*!***********************************************************************
		 \brief
		 Quits the application
		*************************************************************************/
		void Quit();

	private:
		std::shared_ptr<LBEngine> Engine;
	};

	/*!***********************************************************************
	\brief
	Creates the application in the project area, to be used by other projects
	eg. Sandbox
	*************************************************************************/
	Application* CreateApplication(bool EditorMode = false);

}

extern LB::Application* LB::CreateApplication(bool EditorMode);
