#include "ChaserStates.h"
#include "Player/Player.h"

ChaserIdleState::ChaserIdleState(FiniteStateMachine& fsm, std::string name) :
	State(fsm, name)
{
	// State base class is initialized with
	// fsm and IDLE as its params
	// Empty by design
}

 void ChaserIdleState::Enter()
{
	std::cout << "Enter Enemy Idle" << std::endl;
}

 void ChaserIdleState::Update()
 {
	 std::cout << "Update Enemy Idle" << std::endl;
 }
 
 void ChaserIdleState::Exit()
 {
	 std::cout << "Exit Enemy Idle" << std::endl;
 }

 ChaserChaseState::ChaserChaseState(FiniteStateMachine& fsm, std::string name) :
	 State(fsm, name)
 {
	 // State base class is initialized with
	 // fsm and IDLE as its params
	 // Empty by design
 }

 // Chase State functions
 void ChaserChaseState::Enter()
 {
	 std::cout << "Enter Enemy Chase" << std::endl;
 }

 void ChaserChaseState::Update()
 {
	 std::cout << "Update Enemy Chase" << std::endl;
	 PlayerObj->GetComponent<CPRigidBody>()->getPos();
 }

 void ChaserChaseState::Exit()
 {
	 std::cout << "Exit Enemy Chase" << std::endl;
 }
 