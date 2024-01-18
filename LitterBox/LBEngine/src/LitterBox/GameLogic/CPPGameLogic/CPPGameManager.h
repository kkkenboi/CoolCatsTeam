#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSGameManager : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		//Need an array of game objects which will be the enemies to spawn
		//Assign values to each enemy
		//Each wave also has a certain value
		//Spawn enemy and deduct the values

	private:

	};
}