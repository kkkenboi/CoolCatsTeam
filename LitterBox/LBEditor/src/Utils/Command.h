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
	 * 
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
		virtual void Execute() = 0;

		virtual void Undo() = 0;

		virtual bool Merge(std::shared_ptr<ICommand> incomingCommand) = 0;

		virtual CommandType GetType() = 0;

		virtual void OnRemove() = 0;
	};
}
