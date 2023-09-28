
#include "Player.h"

void Player::Initialise()
{
	this->playerObj = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"});

	playerObj->GetComponent<CPTransform>("CPTransform")->SetPosition(LB::Vec2<float>{400.f, 400.f});

	this->playerObj->GetComponent<CPRigidBody>("CPRigidBody")->mNumberID = 1;
	INPUT->SubscribeToKey(MoveUp, KeyCode::KEY_W, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveDown, KeyCode::KEY_S, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveLeft, KeyCode::KEY_A, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveRight, KeyCode::KEY_D, KeyEvent::PRESSED);
}

void Player::Update()
{
	//playerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>());

	//std::cout << "PLAYER POS: " << this->playerObj->GetComponent<CPRigidBody>("CPRigidBody")->getPos().x 
	//	<< " , " << this->playerObj->GetComponent<CPRigidBody>("CPRigidBody")->getPos().y << std::endl;

	//std::cout << "PLAYER TRANS SCALE: " << this->playerObj->GetComponent<CPTransform>("CPTransform")->GetScale().ToString() << std::endl;
	//std::cout << "PLAYER TRANS POS: " << this->playerObj->GetComponent<CPTransform>("CPTransform")->GetPosition().ToString() << std::endl;
}
