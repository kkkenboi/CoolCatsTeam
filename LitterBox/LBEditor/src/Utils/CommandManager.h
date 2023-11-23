/*!************************************************************************
 \file				CommandManager.h
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

#pragma once

#include <memory>
#include <deque>

#include "LitterBox/Core/System.h"
#include "Command.h"

namespace LB
{
	class CommandManager
	{
	public:
		CommandManager();

		void AddCommand(std::shared_ptr<ICommand> newCommand);

		void Undo();

		void Redo();

		void ResizeHistory(int newSize);

	private:
		void AddToHistory(std::shared_ptr<ICommand> newCommand);

		int historyCapacity;	// How many unique commands to store?

		std::deque<std::shared_ptr<ICommand>> history, undoHistory;
	};

	void CheckUndo();

	void CheckRedo();

	extern CommandManager* COMMAND;
}
