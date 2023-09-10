#include "LitterBox.h"

#include <LitterBox/Core/EntryPoint.h>

class Sandbox : public LB::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};


LB::Application* LB::CreateApplication()
{
	return new Sandbox();
}