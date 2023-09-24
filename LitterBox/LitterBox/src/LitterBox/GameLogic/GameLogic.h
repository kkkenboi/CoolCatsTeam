#pragma once
#include "LitterBox/Core/System.h"


namespace LB
{
	class GameLogic : public ISystem
	{
	public:

		GameLogic();
		~GameLogic();

		void Initialize() override;
		void Update() override;
		void Destroy() override;
	private:

	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* GAMELOGIC;

}
