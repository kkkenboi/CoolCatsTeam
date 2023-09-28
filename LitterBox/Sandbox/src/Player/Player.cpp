
#include "Player.h"

GameObject* PlayerObj;

void Player::Initialise()
{
	this->playerObj = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"});

	playerObj->GetComponent<CPTransform>("CPTransform")->SetPosition(LB::Vec2<float>{400.f, 400.f});

	this->playerObj->GetComponent<CPRigidBody>("CPRigidBody")->mNumberID = 1;

	INPUT->SubscribeToKey(MoveUp, KeyCode::KEY_W, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveDown, KeyCode::KEY_S, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveLeft, KeyCode::KEY_A, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveRight, KeyCode::KEY_D, KeyEvent::PRESSED);

	PlayerObj = playerObj;
}

void Player::Update()
{




}

void MoveUp()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{0.f, 150.f});
}
void MoveLeft()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{-150.f, 0.f});
}
void MoveRight()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{150.f, 0.f});
}
void MoveDown()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{0.f, -150.f});
}
