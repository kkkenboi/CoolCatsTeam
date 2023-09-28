#include "SceneTestMain.h"

GameObject* test;

void SceneTestMain::Init()
{
	test = FACTORY->SpawnGameObject({ "CPRender" });
	//test->GetComponent<CPRender>("CPRender")->UpdateTexture(Renderer::GRAPHICS->get_texture("run"));
}

void SceneTestMain::Update()
{

}

void SceneTestMain::Destroy()
{

}
