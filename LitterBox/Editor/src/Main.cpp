#include "LitterBox.h"

#include <LitterBox/Core/EntryPoint.h>

class Editor : public LB::Application
{
public:
	Editor()
	{

	}
	~Editor()
	{

	}
};


LB::Application* LB::CreateApplication()
{
	return new Editor();
}