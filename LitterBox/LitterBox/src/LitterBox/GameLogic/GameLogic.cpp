#include "pch.h"
#include "GameLogic.h"

namespace LB
{
	GameLogic* GAMELOGIC = nullptr;

	GameLogic::GameLogic()
	{
		// Ensure singleton
		if (!GAMELOGIC)
		{
			GAMELOGIC = this;
		}
		else
		{
			std::cerr << "Game Logic already exist\n";
		}
	}

	void GameLogic::Initialize()
	{
		SetSystemName("Game Logic System");
	}

	void GameLogic::Update()
	{

	}

	void GameLogic::Destroy()
	{

	}
}
