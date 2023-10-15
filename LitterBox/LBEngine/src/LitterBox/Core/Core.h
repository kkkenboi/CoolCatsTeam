/*!************************************************************************
 \file				Core.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 
 This file contains functions declarations of the LBEngine class that controls
 the engine and systems. Has a layer system to allow for ImGui implementation
 later on.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "pch.h"
#include "System.h"
#include "LitterBox/Engine/LayerStack.h"

namespace LB 
{
	/*!***********************************************************************
	 \brief
	 The LitterBox Engine that encapsulates all of the systems
	*************************************************************************/
	class LBEngine
	{
	public:

		/*!***********************************************************************
		 \brief
		 Initializes all systems in the game.
		*************************************************************************/
		void Initialise();

		/*!***********************************************************************
		 \brief
		*************************************************************************/
		void GameLoop();

		/*!***********************************************************************
		 \brief
		 Broadcasts a message to all systems.
		*************************************************************************/
		void BroadcastMessage(Message* m);

		/*!***********************************************************************
		 \brief
		 Adds a new system to the game.
		*************************************************************************/
		void AddSystem(ISystem* system);

		/*!***********************************************************************
		 \brief
		 Destroy all systems in reverse order that they were added.
		*************************************************************************/
		void DestroySystems();

		/*!***********************************************************************
		 \brief
		 Checks if the engine is running
		*************************************************************************/
		bool IsRunning() const;

		/*!***********************************************************************
		 \brief
		 Adds a layer from the layerStack
		*************************************************************************/
		void AddLayer(Layer* layer);

		/*!***********************************************************************
		 \brief
		 Removes a layer from the layerStack
		*************************************************************************/
		void RemoveLayer(Layer* layer);

	private:
		std::vector<ISystem*>	Systems;

		bool					m_Running;
	};

	extern LBEngine* CORE; // Global pointer to the singleton
}

