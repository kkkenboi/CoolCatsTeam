#include "ChaserStates.h"

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

 // Chase State functions
 void ChaserChaseState::Enter()
 {
	 std::cout << "Enter Enemy Chase" << std::endl;
 }

 void ChaserChaseState::Update()
 {
	 std::cout << "Update Enemy Chase" << std::endl;
	 //PlayerObj->GetComponent<CPRigidBody>()->getPos();
 }

 void ChaserChaseState::Exit()
 {
	 std::cout << "Exit Enemy Chase" << std::endl;
 }
 