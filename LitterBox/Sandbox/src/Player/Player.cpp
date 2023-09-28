
#include "Player.h"

GameObject* PlayerObj;

void Player::Initialise()
{
	this->playerObj = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"});

	playerObj->GetComponent<CPTransform>("CPTransform")->SetPosition(LB::Vec2<float>{400.f, 400.f});
	playerObj->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));
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
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{0.f, 3000.f * (float)TIME->GetDeltaTime()});
}
void MoveLeft()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{-3000.f * (float)TIME->GetDeltaTime(), 0.f});
}
void MoveRight()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{3000.f * (float)TIME->GetDeltaTime(), 0.f});
}
void MoveDown()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{0.f, -3000.f * (float)TIME->GetDeltaTime()});
}

void SpawnPineapples()
{
	GameObject* clones = FACTORY->SpawnGameObject(ASSETMANAGER->PineappleObject);
	//for (int i{ 0 }; i < 5; ++i)
	//{
	//	clones->GetComponent<CPRigidBody>("CPRigidBody")->mNumberID;
	//	//clones->GetComponent<CPRigidBody>("CPRigidBody")->isStatic = true;
	//	//clones->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));
	//}
	//clones->GetComponent<CPRender>("CPRender")->resetState();
	//clones->StartComponents();
	//GameObject* pClones = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody" });

	//pClones->GetComponent<CPTransform>("CPTransform")->SetPosition(ASSETMANAGER->PineappleObject->GetComponent<CPTransform>("CPTransform")->GetPosition());
	//pClones->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));

}
