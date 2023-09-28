#include "SceneTestMain.h"

GameObject*test, *test2, *test3;

void SceneTestMain::Init()
{
	test = FACTORY->SpawnGameObject({ "CPRender" });
	test2 = FACTORY->SpawnGameObject({ "CPRender" });
	test3 = FACTORY->SpawnGameObject({ "CPRender", "CPRigidBody"}, Vec2<float>(200, 200));
	test3->GetComponent<CPRender>("CPRender")->UpdateTexture(Renderer::GRAPHICS->get_texture("run"));
}

void SceneTestMain::Update()
{

}

void SceneTestMain::Destroy()
{

}
