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
		LBEngine();

		/*!***********************************************************************
		 \brief
		 Initializes all systems in the game.
		*************************************************************************/
		void Initialise();

		/*!***********************************************************************
		 \brief
		 The update loop for all systems
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
		void AddSystem(std::shared_ptr<ISystem> system);

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
		 Returns true if the game is in editor mode, m_editorMode is a bool
		 to check if the editor is attached to the engine.
		*************************************************************************/

		bool IsEditorMode() const;
		/*!***********************************************************************
		 \brief
		 Flips the state of m_editorMode which is a bool to check if the editor 
		 is attached to the engine.
		*************************************************************************/
		void ToggleEditorMode();

		/*!***********************************************************************
		 \brief
		 Sets the state of m_editorMode to the given state, m_editorMode is a bool
		 to check if the the editor is attached to the engine.
		*************************************************************************/
		void SetEditorMode(bool newState);

		// Event to let any subscribers know the editor mode has been toggled
		Event<bool> onEditorModeToggle; 

		/*!***********************************************************************
		 \brief
		 Returns true if the actual game is playing
		*************************************************************************/
		bool IsPlaying() const;

		/*!***********************************************************************
		 \brief
		 Flips the m_isPlaying state
		*************************************************************************/
		void TogglePlaying();

		/*!***********************************************************************
		 \brief
		 Sets the m_isPlaying state to the given one, m_isPlaying checks if the
		 game is actually running (not editor)
		*************************************************************************/
		void SetPlayingMode(bool newState);
		
		// Event to let any subscribers know the game has started/stopped
		Event<bool> onPlayingModeToggle;

	private:
		std::vector<std::shared_ptr<ISystem>> m_systems; // Pointers to all systems currently loaded
		
		bool					m_running;				// Is the engine running?
		bool					m_isPlaying{ false };	// Is the game running?
		bool					m_editorMode{ false };	// Is the editor attached to the game engine?
	};

	extern LBEngine* CORE; // Global pointer to the singleton
}

