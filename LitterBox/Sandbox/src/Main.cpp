#include "LitterBox.h"

class Sandbox : public LitterBox::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};


LitterBox::Application* LitterBox::CreateApplication()
{
	return new Sandbox();
}