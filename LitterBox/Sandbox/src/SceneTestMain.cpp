#include "SceneTestMain.h"

#include "Player/Player.h"

GameObject *test, *test2, *test3;
GameObject *scaleObj, *rotObj, *animObj;

Player* testPlayer;

//place holder animations
//TODO for even later, make an editor that can select custom uv
std::array<std::array<LB::Vec2<float>, 4>, 12> frames[4];

void SceneTestMain::Init()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// GameObject use example

	Renderer::GRAPHICS->create_texture("../Assets/Textures/walk.png", "run");

	test = FACTORY->SpawnGameObject({ "CPRender" });
	test2 = FACTORY->SpawnGameObject({ "CPRender" });
	test3 = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"}, Vec2<float>(200, 200));
	test3->GetComponent<CPRender>("CPRender")->UpdateTexture(Renderer::GRAPHICS->get_texture("run"));

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Rotate and scale test
	scaleObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(600, 600));
	rotObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(800, 600));

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Animation test
	animObj = FACTORY->SpawnGameObject({ "CPRender" }, Vec2<float>(700, 300));
	animObj->GetComponent<CPRender>("CPRender")->texture = Renderer::GRAPHICS->get_texture("run");
	animObj->GetComponent<CPRender>("CPRender")->UpdateTexture(Renderer::GRAPHICS->get_texture("run"));

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Player example
	testPlayer = new Player;
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

}

void SceneTestMain::Destroy()
{
	delete testPlayer;
}
