#include "pch.h"
#include "Components.h"

namespace LB
{
	// Temporary component class for testing, by right these components
	// should belong in their own source files
	class Physics : public IComponent
	{
	public:
		void Initialise() override
		{
			std::cout << "Initialising Physics\n";
		}
		void Serialise() override
		{
			std::cout << "Serialising Physics\n";
		}
		void Update() override
		{
			std::cout << "Updating Physics\n";
		}

		~Physics()
		{
			// delete itself
		};
	private:
		// Should data stay private? 
		int physicsData;
	};

	// Temporary component class for testing, by right these components
	// should belong in their own source files
	class Transform : public IComponent
	{
	public:
		void Initialise() override
		{
			std::cout << "Initialising Transform\n";
		}
		void Serialise() override
		{
			std::cout << "Serialising Transform\n";
		}
		void Update() override
		{
			std::cout << "Updating Transform\n";
		}

		~Transform()
		{
			// delete itself
		};
	private:
		// Should data stay private? 
		int x, y;
		float angle, scale;
	};

	// Temporary component class for testing, by right these components
	// should belong in their own source files
	class Render : public IComponent
	{
	public:
		void Initialise() override
		{
			std::cout << "Initialising Render\n";
		}
		void Serialise() override
		{
			std::cout << "Serialising Render\n";
		}
		void Update() override
		{
			std::cout << "Updating Render\n";
		}

		~Render()
		{
			// delete itself
		};
	private:
		// Should data stay private? 
		int renderData;
	};

}