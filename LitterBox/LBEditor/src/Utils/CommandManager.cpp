/*!************************************************************************
 \file				CommandManager.cpp
 \author(s)
 \par DP email(s):
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file contains

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CommandManager.h"

namespace LB
{
	CommandManager* COMMAND = nullptr;

	CommandManager::CommandManager()
	{
		if (!COMMAND)
			COMMAND = this;
		else
			DebuggerLogError("Command System already exists!");

		ResizeHistory(20);
	}

	void CommandManager::AddCommand(std::shared_ptr<ICommand> newCommand)
	{
		newCommand->Execute();

		if (history.back()->GetType() == newCommand->GetType())
		{
			if (!history.back()->Merge(newCommand))
				AddToHistory(newCommand);
		}
		else
		{
			AddToHistory(newCommand);
		}
	}

	void CommandManager::AddToHistory(std::shared_ptr<ICommand> newCommand)
	{
		// If command history capacity is reached, remove oldest command
		if (history.size() > historySize)
		{
			history.pop_front();
		}
		history.push_back(newCommand);
	}

	void CommandManager::Undo()
	{

	}

	void CommandManager::Redo()
	{

	}

	void CommandManager::ResizeHistory(int newSize)
	{
		historySize = newSize;
		history.resize(historySize);
		undoHistory.resize(historySize);
	}

}
