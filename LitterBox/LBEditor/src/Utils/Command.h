/*!************************************************************************
 \file				Command.h
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

namespace LB
{
	enum CommandType
	{
		MOVE,
		SCALE,
		ROTATE,
		SPAWNGO,
		DELETEGO
	};

	class ICommand
	{
	public:
		virtual void Execute() = 0;

		virtual void Undo() = 0;

		virtual bool Merge(std::shared_ptr<ICommand> incomingCommand) = 0;

		virtual CommandType GetType() = 0;

		virtual ~ICommand() { }
	};
}
