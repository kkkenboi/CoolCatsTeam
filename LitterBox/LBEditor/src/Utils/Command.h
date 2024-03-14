/*!************************************************************************
 \file				Command.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief

 This file contains the abstract class for commands

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

namespace LB
{
	/*!************************************************************************
	 * \brief Command types that are available
	**************************************************************************/
	enum CommandType
	{
		// GameObject Commands
		SPAWNGO,
		DELETEGO,
		// Transform Commands
		MOVE,
		SCALE,
		ROTATE,
		// Render Commands
		RENDERPOS,
		RENDERIMAGE,
		RENDERSSHEET,
		RENDERSSHEETLAYER
		// Collider Commands
	};

	/*!************************************************************************
	 * \brief Abstract base command class for commands
	 * (ALL Commands must implement all of these functions!)
	**************************************************************************/
	class ICommand
	{
	public:
		/*!************************************************************************
		 * \brief Generic execute command, does the intended action
		**************************************************************************/
		virtual void Execute() = 0;

		/*!************************************************************************
		 * \brief Generic undo command, cancels the intended action
		**************************************************************************/
		virtual void Undo() = 0;

		/*!************************************************************************
		 * \brief Tries to merge commands, if merge successful, returns true.
		 * If unable to merge, returns false.
		**************************************************************************/
		virtual bool Merge(std::shared_ptr<ICommand> incomingCommand) = 0;

		/*!************************************************************************
		 * \brief Returns the command type, useful for merging commands
		**************************************************************************/
		virtual CommandType GetType() = 0;

		/*!************************************************************************
		 * \brief If the command needs to do something when this command is
		 * removed from history, add it to this function.
		**************************************************************************/
		virtual void OnRemove() = 0;
	};
}
