#include "LitterBox.h"
#include "LitterBox/Engine/Layer.h"
#include <LitterBox/Core/EntryPoint.h>
class GameplayLayer : public Layer
{
public:
	GameplayLayer(std::string layerName);

	//~GameplayLayer() override;

	//void AttachLayer() override;
	//void DetachLayer() override;

	void UpdateLayer() override;
};

GameplayLayer::GameplayLayer(std::string layerName) : Layer(layerName)
{
	std::cout << "Initialising!\n";
}

void GameplayLayer::UpdateLayer()
{
	std::cout << "Updating!\n";
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

