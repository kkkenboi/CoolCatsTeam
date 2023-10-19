/*!************************************************************************
 \file				Player.cpp
 \author(s)			Ang Jiawei Jarrett | Ryan Tan Jian Hao
 \par DP email(s):	a.jiaweijarrett@digipen.edu | ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				28-09-2023
 \brief
 This file contains the definitions for a player class. This file is a test
 file and simulates a "user created" script that utilises the engine

**************************************************************************/
#include "Player.h"

//Game object that will be the player
GameObject* PlayerObj;
GameObject* testObject;

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
		test[i] = FACTORY->SpawnGameObject({ C_CPRender });
		test[i]->GetComponent<CPRender>()->texture = -1;
	}

	for (int y{ 0 }; y < 50; ++y) {
		for (int x{ 0 }; x < 50; ++x) {
			test[x + y * 50]->GetComponent<CPTransform>()->SetPosition({ x * (WINDOWSSYSTEM->GetWidth() - 100.f) / 50.f + 50.f, y * (WINDOWSSYSTEM->GetHeight() - 100.f) / 50.f + 50.f });
			test[x + y * 50]->GetComponent<CPTransform>()->SetScale({ 0.1f, 0.1f });
		}
	}
}

void SpawnGameObject()
{
	testObject = ASSETMANAGER->PineappleObject;
}

void hide2500() {
	if (!test)
		return;
	for (int i{ 0 }; i < 2500; ++i) {
		test[i]->GetComponent<CPRender>()->set_active();
	}
}

/*!***********************************************************************
\brief
 Initialises the player and all its components (also subscribing key events)
*************************************************************************/
void Player::Initialise()
{
	//auto m = mono_init("LitterBoxEngine.exe");
	this->playerObj = FACTORY->SpawnGameObject({ C_CPRender, C_CPRigidBody, C_CPCollider});

	playerObj->GetComponent<CPTransform>()->SetPosition(LB::Vec2<float>{400.f, 400.f});
	// Test player texture
	playerObj->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("run"));
	playerObj->GetComponent<CPRender>()->play_repeat("up_walk");

	playerObj->GetComponent<CPRigidBody>()->mNumberID = 1;

	// Movement events
	INPUT->SubscribeToKey(MoveUp, KeyCode::KEY_W, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveDown, KeyCode::KEY_S, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveLeft, KeyCode::KEY_A, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(MoveRight, KeyCode::KEY_D, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(RotateClockwise, KeyCode::KEY_E, KeyEvent::PRESSED);
	INPUT->SubscribeToKey(RotateCounterClockwise, KeyCode::KEY_Q, KeyEvent::PRESSED);

	// Explosion event
	INPUT->SubscribeToKey(ExplodeAround, KeyCode::KEY_F, KeyEvent::PRESSED);

	// Anim events
	INPUT->SubscribeToKey(AnimUp, KeyCode::KEY_W, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(AnimRight, KeyCode::KEY_D, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(AnimLeft, KeyCode::KEY_A, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(AnimDown, KeyCode::KEY_S, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(spawn2500, KeyCode::KEY_P, KeyEvent::TRIGGERED);
	INPUT->SubscribeToKey(hide2500, KeyCode::KEY_H, KeyEvent::TRIGGERED);

	PlayerObj = playerObj;
}


/*!***********************************************************************
\brief
 Updates the player
*************************************************************************/
void Player::Update()
{

}

void ExplodeAround()
{
	Vec2<float> current_pos = PlayerObj->GetComponent<CPTransform>()->GetPosition();
	float effect_radius = 200.f;
	float effect_magnitude = 1500.f;

	DEBUG->DrawCircle(current_pos, effect_radius, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});

	std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(current_pos, effect_radius);
	
	std::cout << vec_colliders.size() << std::endl;
	for (size_t i = 0; i < vec_colliders.size(); ++i) {
		Vec2<float> force_to_apply = vec_colliders[i]->m_pos - current_pos;
		force_to_apply = Normalise(force_to_apply) * effect_magnitude;
		
		if (vec_colliders[i]->rigidbody != nullptr)
		{
			if (vec_colliders[i] == PlayerObj->GetComponent<CPCollider>())
			{
				continue;
			}
			vec_colliders[i]->rigidbody->addImpulse(force_to_apply);
		}

	}
}

/*!***********************************************************************
\brief
 up is meant to move the player to North. Meant to be passed as a
 subscriber to Key Press W
*************************************************************************/
void MoveUp()
{
	PlayerObj->GetComponent<CPRigidBody>()->addForce(Vec2<float>{0.f, 5000.f});
}

/*!***********************************************************************
\brief
 left is meant to move the player to the left. Meant to be passed as a
 subscriber to Key Press A
*************************************************************************/
void MoveLeft()
{
	PlayerObj->GetComponent<CPRigidBody>()->addForce(Vec2<float>{-5000.f, 0.f});
}

/*!***********************************************************************
\brief
 func is meant to move the player to the right. Meant to be passed as a
 subscriber to Key Press D
*************************************************************************/
void MoveRight()
{
	PlayerObj->GetComponent<CPRigidBody>()->addForce(Vec2<float>{5000.f, 0.f});
}

/*!***********************************************************************
\brief
 down is meant to move the player to Douth. Meant to be passed as a
 subscriber to Key Press S
*************************************************************************/
void MoveDown()
{
	PlayerObj->GetComponent<CPRigidBody>()->addForce(Vec2<float>{0.f, -5000.f});
}

void RotateClockwise()
{
	PlayerObj->GetComponent<CPRigidBody>()->addRotation(-0.1f);
}

void RotateCounterClockwise()
{
	PlayerObj->GetComponent<CPRigidBody>()->addRotation(0.1f);
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
	PlayerObj->GetComponent<CPRender>()->stop_anim();
	PlayerObj->GetComponent<CPRender>()->play_repeat("up_walk");
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
	PlayerObj->GetComponent<CPRender>()->stop_anim();
	PlayerObj->GetComponent<CPRender>()->play_repeat("left_walk");
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
	PlayerObj->GetComponent<CPRender>()->stop_anim();
	PlayerObj->GetComponent<CPRender>()->play_repeat("right_walk");
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
	PlayerObj->GetComponent<CPRender>()->stop_anim();
	PlayerObj->GetComponent<CPRender>()->play_repeat("down_walk");
}


/*!***********************************************************************
\brief
 Function that is meant to spawn game object prefabs (unused for now)
*************************************************************************/
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
