/*!************************************************************************
 * \file				StateMachine.cpp
 * \author(s)			
 * \par DP email(s):  	@digipen.edu
 * \par Course:       	CSD
 * \date				25/11/2023
 * \brief 
 *  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "StateMachine.h"
#include "LitterBox/Debugging/Debug.h"

/*!***********************************************************************
\brief
The constructor of the base State class
*************************************************************************/
State::State(FiniteStateMachine& FSM, std::string ID) :
	mFSM{ FSM }, mStateID{ ID }
{

}

/*!***********************************************************************
\brief
Returns the StateID which is an std::string of the name of the State
*************************************************************************/
std::string State::GetStateID()
{
	return mStateID;
}

/*!***********************************************************************
\brief
Returns the FiniteStateMachine stored within the State
*************************************************************************/
FiniteStateMachine& State::GetFSM()
{
	return mFSM;
}

// =====
/*!***********************************************************************
\brief
FiniteStateMachine's default constructor
*************************************************************************/
FiniteStateMachine::FiniteStateMachine()
{
	mCurrentState = nullptr;
}

/*!***********************************************************************
\brief
This function allows the user to add a State to the FiniteStateMachine
*************************************************************************/
void FiniteStateMachine::AddState(State* stateType)
{
	if (!(mStateMap.find(stateType->GetStateID()) == mStateMap.end()))
	{
		// Found the key already, so do not make a new key
		return;
	}
	mStateMap[stateType->GetStateID()] = stateType;
}

/*!***********************************************************************
\brief
This function allows the user to get a State from the FiniteStateMachine
through a name
*************************************************************************/
State* FiniteStateMachine::GetState(std::string stateID)
{
	return mStateMap[stateID];
}

/*!***********************************************************************
\brief
This function returns the current state in the FiniteStateMachine
*************************************************************************/
State* FiniteStateMachine::GetCurrentState()
{
	return mCurrentState;
}

/*!***********************************************************************
\brief
This function sets the current State of the FiniteStateMachine
*************************************************************************/
void FiniteStateMachine::SetCurrentState(std::string stateID)
{
	mCurrentState = GetState(stateID);
}

/*!***********************************************************************
\brief
This function handles the transitioning of States from one State to another
State by passing a stateID that has the name of the State
*************************************************************************/
void FiniteStateMachine::ChangeState(std::string stateID)
{
	mCurrentState->Exit();

	mCurrentState = GetState(stateID);

	mCurrentState->Enter();
}

/*!***********************************************************************
\brief
This function is the Update function that runs through the mCurrentState
of the FiniteStateMachine
*************************************************************************/
void FiniteStateMachine::Update()
{
	if (mCurrentState != nullptr)
	{
		mCurrentState->Update();
	}
}

/*!***********************************************************************
\brief
This function is the FixedUpdate function that runs through the mCurrentState
of the FiniteStateMachine
*************************************************************************/
void FiniteStateMachine::FixedUpdate()
{
	if (mCurrentState != nullptr)
	{
		mCurrentState->FixedUpdate();
	}
}