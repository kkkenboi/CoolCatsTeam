#include "SceneTestMain.h"
#include "LitterBox/Engine/Input.h"
#include "Player/Player.h"

GameObject *test, *test2, *test3;
GameObject *scaleObj, *rotObj, *animObj;

Player* testPlayer;

//place holder animations
//TODO for even later, make an editor that can select custom uv
std::array<std::array<LB::Vec2<float>, 4>, 12> frames[4];

//------------------------------MOVEMENT and ANIMATION FUNCTION TEST--------------------------------
void func() {
	Vec2<float> position = animObj->GetComponent<CPTransform>("CPTransform")->GetPosition();
	animObj->GetComponent<CPTransform>("CPTransform")->SetPosition({ position.x + 10.f, position.y });
};

void left() {
	Vec2<float> position = animObj->GetComponent<CPTransform>("CPTransform")->GetPosition();
	animObj->GetComponent<CPTransform>("CPTransform")->SetPosition({ position.x - 10.f, position.y });
};

void up() {
	Vec2<float> position = animObj->GetComponent<CPTransform>("CPTransform")->GetPosition();
	animObj->GetComponent<CPTransform>("CPTransform")->SetPosition({ position.x, position.y + 10.f });
};

void down() {
	Vec2<float> position = animObj->GetComponent<CPTransform>("CPTransform")->GetPosition();
	animObj->GetComponent<CPTransform>("CPTransform")->SetPosition({ position.x, position.y - 10.f });
};

void right_trig() {
	animObj->GetComponent<CPRender>("CPRender")->stop_anim();
	animObj->GetComponent<CPRender>("CPRender")->play_repeat("right_walk");
};

void left_trig() {
	animObj->GetComponent<CPRender>("CPRender")->stop_anim();
	animObj->GetComponent<CPRender>("CPRender")->play_repeat("left_walk");
};

void up_trig() {
	animObj->GetComponent<CPRender>("CPRender")->stop_anim();
	animObj->GetComponent<CPRender>("CPRender")->play_repeat("up_walk");
};

void down_trig() {
	animObj->GetComponent<CPRender>("CPRender")->stop_anim();
	animObj->GetComponent<CPRender>("CPRender")->play_repeat("down_walk");
};
//------------------------------MOVEMENT and ANIMATION FUNCTION TEST--------------------------------

void SceneTestMain::Init()
{
	Renderer::GRAPHICS->create_texture("../Assets/Textures/walk.png", "run");
	Renderer::GRAPHICS->create_texture("../Assets/Textures/test3.png", "pine");
	//---------------------------getting the uvs for the run------------------------
	if (LB::ASSETMANAGER->Textures.find("run") != LB::ASSETMANAGER->Textures.end()) {
		int img_width	{ LB::ASSETMANAGER->Textures.find("run")->second.first->width };
		int img_height	{ LB::ASSETMANAGER->Textures.find("run")->second.first->height };

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

	test = FACTORY->SpawnGameObject({ "CPRender" });
	test2 = FACTORY->SpawnGameObject({ "CPRender" });
	test3 = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"}, Vec2<float>(200, 200));
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
	animObj->GetComponent<CPRender>("CPRender")->play_repeat("up_walk");
	LB::INPUT->SubscribeToKey(func, LB::KeyCode::KEY_D, LB::KeyEvent::PRESSED);
	LB::INPUT->SubscribeToKey(left, LB::KeyCode::KEY_A, LB::KeyEvent::PRESSED);
	LB::INPUT->SubscribeToKey(up, LB::KeyCode::KEY_W, LB::KeyEvent::PRESSED);
	LB::INPUT->SubscribeToKey(down, LB::KeyCode::KEY_S, LB::KeyEvent::PRESSED);
	LB::INPUT->SubscribeToKey(right_trig, LB::KeyCode::KEY_D,LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(left_trig, LB::KeyCode::KEY_A,LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(up_trig, LB::KeyCode::KEY_W,LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(down_trig, LB::KeyCode::KEY_S,LB::KeyEvent::TRIGGERED);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Player example
	testPlayer = new Player;
	testPlayer->Initialise();
	LB::INPUT->SubscribeToKey(PlayTestSound, LB::KeyCode::KEY_W, LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(PlayTestSound, LB::KeyCode::KEY_S, LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(PlayAHHSound, LB::KeyCode::KEY_A, LB::KeyEvent::TRIGGERED);
	LB::INPUT->SubscribeToKey(PlayExplosionSound, LB::KeyCode::KEY_D, LB::KeyEvent::TRIGGERED);
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
