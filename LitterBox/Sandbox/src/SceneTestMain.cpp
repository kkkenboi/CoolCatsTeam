/*!************************************************************************
 \file				SceneTestMain.cpp
 \author(s)			Ang Jiawei Jarrett | Ryan Tan Jian Hao
 \par DP email(s):	a.jiaweijarrett@digipen.edu | ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				28-09-2023
 \brief
 This file contains the first test script that will be used to develop the
 game.

**************************************************************************/
#include "SceneTestMain.h"
#include "LitterBox/Engine/Input.h"
#include "Player/Player.h"

GameObject*test2, *test3, *static_wall;
GameObject *scaleObj, *rotObj, *animObj;

GameObject** test;

Player* testPlayer;
//-------------------------Scene Objects----------------------------

//place holder animations
//TODO for even later, make an editor that can select custom uv
std::array<std::array<LB::Vec2<float>, 4>, 12> frames[4];

bool paused = false;
void TogglePause()
{
	paused = !paused;
	TIME->Pause(paused);
}

/*!***********************************************************************
 \brief
 Initialises the current scene (for now it contains a lot of tests)
*************************************************************************/
void SceneTestMain::Init()
{
	Renderer::GRAPHICS->create_texture("../Assets/Textures/walk.png", "run");
	Renderer::GRAPHICS->create_texture("../Assets/Textures/test3.png", "pine");
	Renderer::GRAPHICS->create_texture("../Assets/Textures/cat.png", "cat");

	/////////////////////////////////////////////////////////////////////////////////////////////
	// GameObject use example
	/*test2 = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody" });
	test2->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("cat"));

	test3 = FACTORY->SpawnGameObject({ "CPRender" , "CPRigidBody"}, Vec2<float>(200, 200));
	test3->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("cat"));*/

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Rotate and scale test
	scaleObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(600, 600));
	scaleObj->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));

	rotObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(800, 600));
	rotObj->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Animation test
	//---------------------------getting the uvs for the run------------------------
	if (LB::ASSETMANAGER->Textures.find("run") != LB::ASSETMANAGER->Textures.end()) {
		int img_width{ LB::ASSETMANAGER->Textures.find("run")->second.first->width };
		int img_height{ LB::ASSETMANAGER->Textures.find("run")->second.first->height };

		float x_inc{ (float)img_width / (12.f * (float)img_width) };
		float y_inc{ (float)img_height / (4.f * (float)img_height) };

		for (int y{ 0 }; y < 4; ++y)
			for (int x{ 0 }; x < 12; ++x) {
				frames[y][x].at(0) = { x * x_inc, y * y_inc };//bottom left
				frames[y][x].at(1) = { (x + 1) * x_inc, y * y_inc };//bottom right
				frames[y][x].at(2) = { (x + 1) * x_inc, (y + 1) * y_inc };//top right
				frames[y][x].at(3) = { x * x_inc, (y + 1) * y_inc };//top left
			}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// GameObject use example
	//test = FACTORY->SpawnGameObject({ "CPRender" });
	test2 = FACTORY->SpawnGameObject({ "CPRender" });
	test3 = FACTORY->SpawnGameObject({ "CPRender" , "CPRigidBody"}, Vec2<float>(200, 200));
	test3->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Rotate and scale test
	scaleObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(600, 600));
	rotObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(800, 600));

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Animation test
	animObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(700, 300));
	animObj->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("run"));

	Renderer::GRAPHICS->init_anim("up_walk", frames[0].data(), 0.5f, 12);
	Renderer::GRAPHICS->init_anim("right_walk", frames[1].data(), 0.5f, 12);
	Renderer::GRAPHICS->init_anim("left_walk", frames[2].data(), 0.5f, 12);
	Renderer::GRAPHICS->init_anim("down_walk", frames[3].data(), 0.5f, 12);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Pause & Debug example
	LB::INPUT->SubscribeToKey(TogglePause, KeyCode::KEY_U, LB::KeyEvent::TRIGGERED);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Player example
	testPlayer = new Player;
	testPlayer->Initialise();
	LB::INPUT->SubscribeToKey(PlayTestSound, LB::KeyCode::KEY_W, LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(PlayTestSound, LB::KeyCode::KEY_S, LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(PlayAHHSound, LB::KeyCode::KEY_A, LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(PlayExplosionSound, LB::KeyCode::KEY_D, LB::KeyEvent::TRIGGERED);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Prefab example
	LB::INPUT->SubscribeToKey(SpawnPineapples, LB::KeyCode::KEY_Q, LB::KeyEvent::TRIGGERED);

	static_wall = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody" }, Vec2<float>(200, 600));
	static_wall->GetComponent<CPRigidBody>("CPRigidBody")->isStatic = true;
	static_wall->GetComponent<CPRender>("CPRender")->UpdateTexture(LB::ASSETMANAGER->GetTextureIndex("pine"));

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Crash logging test (Uncomment code below for a crash!!!)
	//int* test = nullptr;
	//*test = 5;

	//JSONSerializer stream;
	//stream.DeserializeFromFile("TestObject", *testPlayer->playerObj);
	//std::cout << testPlayer->playerObj->GetComponent<CPTransform>("CPTransform")->GetPosition().ToString();
}

/*!***********************************************************************
 \brief
 Updates the current Scene 
*************************************************************************/
void SceneTestMain::Update()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// Rotate and scale test
	static float degree{ 0.f };
	scaleObj->GetComponent<CPTransform>("CPTransform")->SetScale({ sinf(degree) ,sinf(degree) });
	rotObj->GetComponent<CPTransform>("CPTransform")->SetRotation(degree);

	degree = degree > 6.28318531f ? 0.f : degree + 0.01f;
	/////////////////////////////////////////////////////////////////////////////////////////////
	testPlayer->Update();
}

/*!***********************************************************************
 \brief
 Functions that will be called when the scene is destroyed
*************************************************************************/
void SceneTestMain::Destroy()
{
	//delete testPlayer;
	//This test lets us know that we can "save" the player's position
	JSONSerializer stream;
	stream.SerializeToFile("TestObject", *testPlayer->playerObj);
}
