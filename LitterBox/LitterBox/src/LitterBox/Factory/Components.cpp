#include "pch.h"
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

}