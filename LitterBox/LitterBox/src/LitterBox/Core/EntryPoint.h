#pragma once
#include "pch.h"

extern LB::Application* LB::CreateApplication();

int main(int argc, char** argv)
{
	auto app = LB::CreateApplication();
	app->Run();
	delete app;

}