#pragma once
#include "LitterBox/Core/System.h"


namespace LB
{
	class GameLogic : public ISystem
	{
	public:
		void Initialize() override;
		void Update() override;
		void Destroy() override;
		virtual std::string GetName() override { return "GameLogic"; }
	private:

	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* GAMELOGIC;

}
