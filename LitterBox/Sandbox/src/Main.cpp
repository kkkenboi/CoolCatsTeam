#include "LitterBox.h"
#include "LitterBox/Engine/Layer.h"
#include <LitterBox/Core/EntryPoint.h>

using namespace LB;

class GameplayLayer : public Layer
{
public:
	GameplayLayer(std::string layerName);

	//~GameplayLayer() override;

	//void AttachLayer() override;
	//void DetachLayer() override;

	void UpdateLayer() override;
private:
	LB::GameObject* testGameObj1{};
};

GameplayLayer::GameplayLayer(std::string layerName) : Layer(layerName)
{
	// Initialise
	testGameObj1 = FACTORY->CreateGameObject();
	if (testGameObj1->GetID() == 0) 		// ID only starts at 1
	{
		testGameObj1->SetID(FACTORY->GetLastObjID());
	}
	testGameObj1->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
	testGameObj1->AddComponent("CPRender", FACTORY->GetCMs()["CPRender"]->Create());
	testGameObj1->AddComponent("CPRigidBody", FACTORY->GetCMs()["CPRigidBody"]->Create());
	testGameObj1->StartComponents();

	// Sends game object to the Game Object Manager
	// For now, push back in the function to go manager,
	// However, in the future we might need to change when we need to render a lot in one go
	// or send a equal amount of game objects at one go
	// Might be redundant too because we should initialize a pool at the start
	GOMANAGER->AddGameObject(testGameObj1);
}

void GameplayLayer::UpdateLayer()
{
	// Update Loop
	std::cout << TIME->GetTime() << '\n';
	if (TIME->GetTime() > 5.00)
	{
		testGameObj1->GetComponent<CPTransform>("CPTransform")->SetPosition({ 800.f,450.f });
		std::cout << testGameObj1->GetComponent<CPTransform>("CPTransform")->GetPosition().x << '\n';
	}

}

class Sandbox : public LB::Application
{
public:
	Sandbox()
	{
		LB::CORE->AddLayer(new GameplayLayer("Engine"));
	}
	~Sandbox()
	{

	}
};

LB::Application* LB::CreateApplication()
{
	return new Sandbox();
}


int main(int argc, char** argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	auto app = LB::CreateApplication();
	app->Run();
	delete app;

}

