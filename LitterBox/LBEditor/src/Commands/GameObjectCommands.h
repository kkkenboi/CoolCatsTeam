/*!************************************************************************
 \file				GameObjectCommands.cpp
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
#include "Utils/Command.h"
#include "LitterBox/Factory/GameObjectManager.h"

namespace LB
{
	class RemoveObjectCommand : public ICommand
	{
	public:
		RemoveObjectCommand(GameObject* GOToRemove) : removedGO{ GOToRemove } { }

		void Execute() override
		{

		}

		void Undo() override
		{

		}

		bool Merge(std::shared_ptr<ICommand> incomingCommand) override
		{
			return false; // This command can never merge
		}

		CommandType GetType() override
		{
			return DELETEGO;
		}

	private:
		GameObject* removedGO;
	};

}
