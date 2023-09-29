
#include "Player.h"

GameObject* PlayerObj;

extern GameObject** test;
/*!***********************************************************************
\brief
 Temp function to create and display the 2500 object test. Is mapped to
 P. It will only spawn it once and it cannot be despawned.
*************************************************************************/
void spawn2500() {
	if (test) {
		return;
	}

	test = new GameObject * [2500];
	for (int i{ 0 }; i < 2500; ++i) {
		test[i] = FACTORY->SpawnGameObject({ "CPRender" });
	}

	for (int y{ 0 }; y < 50; ++y) {
		for (int x{ 0 }; x < 50; ++x) {
			test[x + y * 50]->GetComponent<CPTransform>("CPTransform")->SetPosition({ x * (WINDOWSSYSTEM->GetWidth() - 100.f) / 50.f + 50.f, y * (WINDOWSSYSTEM->GetHeight() - 100.f) / 50.f + 50.f });
			test[x + y * 50]->GetComponent<CPTransform>("CPTransform")->SetScale({ 0.1f, 0.1f });
		}
	}
}


void Player::Initialise()
{
	this->playerObj = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"});

	playerObj->GetComponent<CPTransform>("CPTransform")->SetPosition(LB::Vec2<float>{400.f, 400.f});
	// Test player texture
	playerObj->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("run"));
	playerObj->GetComponent<CPRender>("CPRender")->play_repeat("up_walk");

	playerObj->GetComponent<CPRigidBody>("CPRigidBody")->mNumberID = 1;

	// Movement events
	INPUT->SubscribeToKey(MoveUp, KeyCode::KEY_W, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveDown, KeyCode::KEY_S, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveLeft, KeyCode::KEY_A, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveRight, KeyCode::KEY_D, KeyEvent::PRESSED);

	// Anim events
	INPUT->SubscribeToKey(AnimUp, KeyCode::KEY_W, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(AnimRight, KeyCode::KEY_D, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(AnimLeft, KeyCode::KEY_A, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(AnimDown, KeyCode::KEY_S, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(spawn2500, KeyCode::KEY_P, KeyEvent::TRIGGERED);

	PlayerObj = playerObj;
}

void Player::Update()
{

}

/*!***********************************************************************
\brief
 up is meant to move the player to North. Meant to be passed as a
 subscriber to Key Press W
*************************************************************************/
void MoveUp()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{0.f, 3000.f * (float)TIME->GetDeltaTime()});
}

/*!***********************************************************************
\brief
 left is meant to move the player to the left. Meant to be passed as a
 subscriber to Key Press A
*************************************************************************/
void MoveLeft()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{-3000.f * (float)TIME->GetDeltaTime(), 0.f});
}

/*!***********************************************************************
\brief
 func is meant to move the player to the right. Meant to be passed as a
 subscriber to Key Press D
*************************************************************************/
void MoveRight()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{3000.f * (float)TIME->GetDeltaTime(), 0.f});
}

/*!***********************************************************************
\brief
 down is meant to move the player to Douth. Meant to be passed as a
 subscriber to Key Press S
*************************************************************************/
void MoveDown()
{
	PlayerObj->GetComponent<CPRigidBody>("CPRigidBody")->addForce(Vec2<float>{0.f, -3000.f * (float)TIME->GetDeltaTime()});
}

/*!***********************************************************************
\brief
 up_trig is meant to queue the up playing animation once when the W
 key is triggered.

 NOTE: if you put this in the Event::Pressed I will kill you when the game
 starts to run slow.
*************************************************************************/
void AnimUp()
{
	PlayerObj->GetComponent<CPRender>("CPRender")->stop_anim();
	PlayerObj->GetComponent<CPRender>("CPRender")->play_repeat("up_walk");
}

/*!***********************************************************************
\brief
 left_trig is meant to queue the left playing animation once when the A
 key is triggered.

 NOTE: if you put this in the Event::Pressed I will kill you when the game
 starts to run slow.
*************************************************************************/
void AnimLeft()
{
	PlayerObj->GetComponent<CPRender>("CPRender")->stop_anim();
	PlayerObj->GetComponent<CPRender>("CPRender")->play_repeat("left_walk");
}

/*!***********************************************************************
\brief
 right_trig is meant to queue the right playing animation once when the D
 key is triggered.

 NOTE: if you put this in the Event::Pressed I will kill you when the game
 starts to run slow.
*************************************************************************/
void AnimRight()
{
	PlayerObj->GetComponent<CPRender>("CPRender")->stop_anim();
	PlayerObj->GetComponent<CPRender>("CPRender")->play_repeat("right_walk");
}

/*!***********************************************************************
\brief
 down_trig is meant to queue the down playing animation once when the S
 key is triggered.

 NOTE: if you put this in the Event::Pressed I will kill you when the game
 starts to run slow.
*************************************************************************/
void AnimDown()
{
	PlayerObj->GetComponent<CPRender>("CPRender")->stop_anim();
	PlayerObj->GetComponent<CPRender>("CPRender")->play_repeat("down_walk");
}

void SpawnPineapples()
{
	 //clone = FACTORY->SpawnGameObject(ASSETMANAGER->PineappleObject);
	//GameObject* clone2 = FACTORY->SpawnGameObject(ASSETMANAGER->PineappleObject);
	//GameObject* clone = FACTORY->CreateGameObject();
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
