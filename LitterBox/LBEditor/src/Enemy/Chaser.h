#pragma once
#include "LitterBox.h"
#include "LitterBox/GameLogic/StateMachine.h"
#include "ChaserStates.h"

class Chaser 
{
public:
	LB::GameObject* chaserObj;
	
	FiniteStateMachine mChaserFSM;
public:
	void Initialise();

	void Update();

	void Destroy();
};