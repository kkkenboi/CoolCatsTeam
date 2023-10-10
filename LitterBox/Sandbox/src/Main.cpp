#include "LitterBox.h"
#include "SceneTestMain.h"

class GameplayLayer : public LB::Layer
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
}

void GameplayLayer::UpdateLayer()
{
}

class Sandbox : public LB::Application
{
public:
	Sandbox() : Application(new SceneTestMain)
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

