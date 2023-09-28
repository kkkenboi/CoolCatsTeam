#include "LitterBox.h"
#include "LitterBox/Engine/Layer.h"
#include <LitterBox/Core/EntryPoint.h>

//using namespace LB;
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
	LB::Text sampleText{};
	Renderer::Camera defaultCam{};
};

GameplayLayer::GameplayLayer(std::string layerName) : Layer(layerName)
{
	sampleText.LoadFont("../Assets/Fonts/KernlGrotesk.otf");

	// Initialise
	testGameObj1 = LB::FACTORY->CreateGameObject();
	if (testGameObj1->GetID() == 0) 		// ID only starts at 1
	{
		testGameObj1->SetID(LB::FACTORY->GetLastObjID());
	}
	testGameObj1->AddComponent("CPTransform", LB::FACTORY->GetCMs()["CPTransform"]->Create());
	testGameObj1->AddComponent("CPRender", LB::FACTORY->GetCMs()["CPRender"]->Create());
	testGameObj1->AddComponent("CPRigidBody", LB::FACTORY->GetCMs()["CPRigidBody"]->Create());
	testGameObj1->StartComponents();

	// Sends game object to the Game Object Manager
	// For now, push back in the function to go manager,
	// However, in the future we might need to change when we need to render a lot in one go
	// or send a equal amount of game objects at one go
	// Might be redundant too because we should initialize a pool at the start
	LB::GOMANAGER->AddGameObject(testGameObj1);
}

void GameplayLayer::UpdateLayer()
{
	// Update Loop
	std::cout << LB::TIME->GetTime() << '\n';
	if (LB::TIME->GetTime() > 5.00)
	{
		testGameObj1->GetComponent<LB::CPTransform>("CPTransform")->SetPosition({ 800.f,450.f });
		std::cout << testGameObj1->GetComponent<LB::CPTransform>("CPTransform")->GetPosition().x << '\n';
	}

	sampleText.RenderText("Hello pls work i am beggin, beggin uuuuuuu\n", {800.f,450.f}, 1.f);

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

