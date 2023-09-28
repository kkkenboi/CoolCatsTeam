#include "LitterBox.h"

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

int main()
{
	return 0;
}

LB::Application* LB::CreateApplication()
{
	return new Editor();
}