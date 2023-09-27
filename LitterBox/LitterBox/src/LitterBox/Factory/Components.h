#pragma once
#include <Litterbox/Engine/Message.h>
#include <LitterBox/Debugging/Memory.h>
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Factory/GameObjectManager.h"

namespace LB
{
	// --can be changed to be serialised--
	enum ComponentTypeID
	{
		C_CPNone = 0,
		C_CPRigidBody,
		C_CPTransform,
		C_CPRender
	};

	// Interface for derived components to use as a base
	class IComponent
	{
	public:

		virtual void Initialise() = 0;
		virtual void Serialise() = 0;
		virtual void Deserialise() = 0;
		virtual void Destroy() = 0;
		// To destruct all other derived components

		ComponentTypeID GetType()	{ return TypeID; }

		ComponentTypeID TypeID		{ C_CPNone };
		GameObject* gameObj			{ nullptr };
	//protected:
	};

	class CPTransform : public IComponent
	{
	public:
		void Initialise() override
		{
			// In the future, should be based on deserialised data from 
			// data files or user input
			double posx{}, posy{};
			glfwGetCursorPos(WINDOWSSYSTEM->GetWindow(), &posx, &posy);

			xPos = posx;
			yPos = posy;
			std::cout << "Initialising Transform\n";
		}
		void Serialise() override
		{
			std::cout << "Serialising Transform\n";
		}
		void Deserialise() override
		{
			std::cout << "Deserialising Transform\n";
		}
		void Destroy() override
		{
			std::cout << "Destroying Transform\n";
		}

	private:
		// Should data stay private? 
		int xPos, yPos;
		float angle, scale;
	};

	class CPRender : public IComponent
	{
	public:
		void Initialise() override
		{
			double posx{}, posy{};
			glfwGetCursorPos(WINDOWSSYSTEM->GetWindow(), &posx, &posy);

			Renderer::render_Object* temp = new Renderer::render_Object;
			temp->position.x = posx;
			temp->position.y = WINDOWSSYSTEM->GetHeight() - posy;

			temp->w = 100.f;
			temp->h = 100.f;


			renderObj = temp;
			std::cout << "Render component initialising mouse position values\n";
		}
		void Serialise() override
		{
			std::cout << "Serialising Render\n";
		}
		void Deserialise() override
		{
			std::cout << "Deserialising Render\n";
		}
		void Destroy() override
		{
			//delete renderObj;
		}

	private:
		// Should data stay private? 
		Renderer::render_Object* renderObj;
	};



	// Interface to make components and tag IDs
	class ComponentMaker
	{
	public:
		ComponentMaker(ComponentTypeID typeId) : TypeID(typeId) {}

		ComponentTypeID TypeID;
		///Create the component
		virtual IComponent* Create() = 0;
		virtual ~ComponentMaker() {};

	};

	// Class Template to create different types of ComponentMaker
	// ComponentType here refers to the derived classes of IComponent
	template<typename ComponentType>
	class ComponentMakerType : public ComponentMaker
	{
	public:
		ComponentMakerType(ComponentTypeID typeId) : ComponentMaker(typeId) {}

		IComponent* Create() override
		{
			// Returns a constructor to the derived class of IComponent
			return new ComponentType();
		}
	};

}

#define CreateComponentMaker(ComponentType) FACTORY->InitCM ( #ComponentType, new ComponentMakerType<ComponentType>( C_##ComponentType ) );