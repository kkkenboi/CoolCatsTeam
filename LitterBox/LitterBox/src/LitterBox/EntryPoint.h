#pragma once
#include "pch.h"
extern LitterBox::Application* LitterBox::CreateApplication();

int main(int argc, char** argv)
{
	auto app = LitterBox::CreateApplication();
	app->Run();
	delete app;

}