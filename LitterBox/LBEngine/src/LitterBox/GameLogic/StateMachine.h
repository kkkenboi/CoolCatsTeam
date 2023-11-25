/*!************************************************************************
 * \file				StateMachine.h
 * \author(s)			
 * \par DP email(s):  	@digipen.edu
 * \par Course:       	CSD
 * \date				25/11/2023
 * \brief 
 *  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
//#include "LitterBox/Engine/Input.h"
#include <map>
#include <memory>
#include <string>

// Forward declaration of FiniteStateMachine class
class FiniteStateMachine;

// T here would be like a
// scoped enum class type such as T = CHASERENEMY
// which would have enums like IDLE, CHASE, etc...
class State
{
public:
	// Stores the FSM that it is located in, allowing
	// for the change of states within the FSM

	/*!***********************************************************************
	\brief
	The constructor of the base State class
	*************************************************************************/
	State(FiniteStateMachine& FSM, std::string ID);

	/*!***********************************************************************
	\brief
	Returns the StateID which is an std::string of the name of the State
	*************************************************************************/
	std::string GetStateID();

	/*!***********************************************************************
	\brief
	Returns the FiniteStateMachine stored within the State
	*************************************************************************/
	FiniteStateMachine& GetFSM();

	// Virtual functions
	/*!***********************************************************************
	\brief
	Defaults the destructor of the virtual destructor of the State class
	*************************************************************************/
	virtual ~State() = default;
	
	/*!***********************************************************************
	\brief
	The Enter() state is a pure virtual function that must be overridden
	by the child class inheriting the base
	*************************************************************************/
	virtual void Enter() = 0;


	/*!***********************************************************************
	\brief
	The Update() state is a pure virtual function that must be overridden
	by the child class inheriting the base
	*************************************************************************/
	virtual void Update() = 0;

	/*!***********************************************************************
	\brief
	The Exit() state is a pure virtual function that must be overridden
	by the child class inheriting the base
	*************************************************************************/
	virtual void Exit() = 0;



private:
	FiniteStateMachine& mFSM;
	std::string mStateID;
};

// Consists of a set of states
// Only can be in one state at a time
class FiniteStateMachine 
{
public:
	/*!***********************************************************************
	\brief
	FiniteStateMachine's default constructor
	*************************************************************************/
	FiniteStateMachine();

	// Add a state
	/*!***********************************************************************
	\brief
	This function allows the user to add a State to the FiniteStateMachine
	*************************************************************************/
	void AddState(State* stateType);

	/*!***********************************************************************
	\brief
	This function allows the user to get a State from the FiniteStateMachine
	through a name
	*************************************************************************/
	State* GetState(std::string stateID);

	State* GetCurrentState();

	/*!***********************************************************************
	\brief
	This function sets the current State of the FiniteStateMachine
	*************************************************************************/
	void SetCurrentState(std::string stateID);

	/*!***********************************************************************
	\brief
	This function handles the transitioning of States from one State to another
	State by passing a stateID that has the name of the State
	*************************************************************************/
	void ChangeState(std::string stateID);
	
	/*!***********************************************************************
	\brief
	This function is the Update function that runs through the mCurrentState
	of the FiniteStateMachine
	*************************************************************************/
	void Update();

private:
	// Map that contains all the states within this given FiniteStateMachine
	std::map<std::string, State*> mStateMap;

	State* mCurrentState;
};
