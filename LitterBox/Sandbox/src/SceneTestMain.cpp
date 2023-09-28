#include "SceneTestMain.h"

#include "Player/Player.h"

GameObject *test, *test2, *test3;
GameObject *scaleObj, *rotObj, *animObj;

Player* testPlayer;

void SceneTestMain::Init()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// GameObject use example

	test = FACTORY->SpawnGameObject({ "CPRender" });
	test2 = FACTORY->SpawnGameObject({ "CPRender" , "CPRigidBody"});
	test3 = FACTORY->SpawnGameObject({ "CPRender"}, Vec2<float>(200, 200));
	test3->GetComponent<CPRender>("CPRender")->UpdateTexture(Renderer::GRAPHICS->get_texture("run"));

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Rotate and scale test
	scaleObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(600, 600));
	rotObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(800, 600));

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Player example
	testPlayer = new Player;
	testPlayer->Initialise();
}

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

void SceneTestMain::Destroy()
{
	//delete testPlayer;
}
