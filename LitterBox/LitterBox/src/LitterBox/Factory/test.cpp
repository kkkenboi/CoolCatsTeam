#include "pch.h"
#include "LitterBox/Renderer/Renderer.h"
#include "Platform/Windows/Windows.h"
#include "Components.h"

namespace LB
{
	// Temporary component class for testing, by right these components
	// should belong in their own source files
	//class Physics : public IComponent
	//{
	//public:
	//	void Initialise() override
	//	{
	//		std::cout << "Initialising Physics\n";
	//	}
	//	void Serialise() override
	//	{
	//		std::cout << "Serialising Physics\n";
	//	}
	//	void Deserialise() override
	//	{
	//		std::cout << "Deserialising Physics\n";
	//	}
	//	void Destroy() override
	//	{
	//		std::cout << "Destroying Physics\n";
	//	}

	//private:
	//	// Should data stay private? 
	//	int physicsData;
	//};

	//class Transform : public IComponent
	//{
	//public:
	//	void Initialise() override
	//	{
	//		std::cout << "Initialising Transform\n";
	//	}
	//	void Serialise() override
	//	{
	//		std::cout << "Serialising Transform\n";
	//	}
	//	void Deserialise() override
	//	{
	//		std::cout << "Deserialising Transform\n";
	//	}
	//	void Destroy() override
	//	{
	//		std::cout << "Destroying Transform\n";
	//	}

	//private:
	//	// Should data stay private? 
	//	int x, y;
	//	float angle, scale;
	//};

	//struct vec2 {
	//	float x, y;
	//};

	//struct vec3 {
	//	float x, y, z;
	//};

	//class Render : public IComponent
	//{
	//public:
	//	void Initialise() override
	//	{
	//		double posx{}, posy{};
	//		glfwGetCursorPos(WINDOWSSYSTEM->GetWindow(), &posx, &posy);

	//		Renderer::render_Object* temp = new Renderer::render_Object;
	//		temp->position.x = posx;
	//		temp->position.y = posy;

	//		temp->w = 1000.f;
	//		temp->h = 1000.f;

	//		temp->col = { 1.f, 0.f, 0.f };

	//		renderObj = temp;
	//		std::cout << "Render component initialising mouse position values\n";
	//	}
	//	void Serialise() override
	//	{
	//		std::cout << "Serialising Render\n";
	//	}
	//	void Deserialise() override
	//	{
	//		std::cout << "Deserialising Render\n";
	//	}
	//	void Destroy() override
	//	{
	//		//delete renderObj;
	//	}

	//private:
	//	// Should data stay private? 
	//	Renderer::render_Object* renderObj;
	//};

}