/*!************************************************************************
 \file				CommandManager.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file contains the command manager class which will handle all the 
 commands done during the editor runtime. Any commands done must be
 added to the command manager to be tracked.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <memory>
#include <deque>

#include "LitterBox/Core/System.h"
#include "LitterBox/Scene/Scene.h"
#include "Command.h"

namespace LB
{
	/*!***********************************************************************
	  \brief
	  The CommandManager handles all commands done during the editor runtime.
	*************************************************************************/
	class CommandManager
	{
	public:
		/*!***********************************************************************
		 \brief
		 Constructor, sets up the CommandManager and subscribes to the key presses
		 to enable undoing and redoing.
		*************************************************************************/
		CommandManager();

		/*!***********************************************************************
		 \brief
		 Whenever a command (action) is done, the command is added to the manager
		 through this function.
		*************************************************************************/
		void AddCommand(std::shared_ptr<ICommand> newCommand);

		/*!***********************************************************************
		 \brief
		 Upon the undo key press, the last command given will be undone and it
		 will be stored on the redo history.
		*************************************************************************/
		void Undo();

		/*!***********************************************************************
		 \brief
		 If there are any commands undone, upon the redo key press, the command
		 will be executed again.
		*************************************************************************/
		void Redo();

		/*!***********************************************************************
		 \brief
		 Changes how many commands to store in memory based on the newSize given,
		 if the newSize is smaller than the current size, the oldest commands
		 will be deleted.
		*************************************************************************/
		void ResizeHistory(int newSize);

		/*!***********************************************************************
		 \brief
		 Clears any command saved.
		*************************************************************************/
		void ClearHistory();

		/*!***********************************************************************
		 \brief
		 Clears any commands undone (Saved to redo history).
		*************************************************************************/
		void ClearRedoHistory();

		/*!***********************************************************************
		 \brief
		 Check if there are there any command changes to the current scene 
		 that are not saved.
		*************************************************************************/
		bool UpToDate();

		/*!***********************************************************************
		 \brief
		 For the command manager to check if any commands are not saved, the index
		 of the latest command needs to be updated here.
		*************************************************************************/
		void UpdateCommandsSaved();

		/*!***********************************************************************
		 \brief
		 Returns the last command send to the command manager.
		*************************************************************************/
		std::shared_ptr<ICommand> GetLastCommand();

		/*!***********************************************************************
		 \brief
		 Returns all the 'undone' commands currently stored in the redo history.
		*************************************************************************/
		std::deque<std::shared_ptr<ICommand>>& GetRedoHistory();

	private:
		/*!***********************************************************************
		 \brief
		 Internal function for command manager to add new commands, if the size of
		 commands stored exceeds the capacity, the command manager will remove
		 the oldest one.
		*************************************************************************/
		void AddToHistory(std::shared_ptr<ICommand> newCommand);

		int m_historyCapacity;		// How many unique commands to store?
		int m_savedCommandIndex{0};	// The index of the last command saved

		std::deque<std::shared_ptr<ICommand>> history, undoHistory;
	};

	/*!***********************************************************************
	 \brief
	 For event subscriptions, these global functions call the same function
	 in the command manager.
	*************************************************************************/
	void CheckUndo();
	void CheckRedo();
	void UpdateCommandsSaved();
	void ClearHistory(bool isPlaying);

	// Extern singleton pointer
	extern CommandManager* COMMAND;
}
