#pragma once
#include <Litterbox/Engine/Message.h>
#include <LitterBox/Debugging/Memory.h>
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Utils/Math.h"

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
		virtual void Initialise() {};
		virtual void Serialise() {};
		virtual void Update() {};
		virtual void Deserialise() {};
		virtual void Destroy() {};
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

		Vec2<float> GetPosition() const
		{
			return pos;
		}

		void SetPosition(Vec2<float> const& newPos)
		{
			pos = newPos;
		}

		Vec2<float> GetScale() const
		{
			return scale;
		}

		void SetScale(Vec2<float> const& newScale)
		{
			scale = newScale;
		}

		float GetRotation() const
		{
			return angle;
		}

		void SetRotation(float newRotation)
		{
			angle = newRotation;
		}

	private:
		Vec2<float> pos{}, scale{};
		float angle{};
	};

	class CPRender : public IComponent
	{
	public:
		void Initialise() override
		{
			double posx{}, posy{};
			glfwGetCursorPos(WINDOWSSYSTEM->GetWindow(), &posx, &posy);

			Renderer::render_Object* temp = new Renderer::render_Object;
			temp->position.x = (float)posx;
			temp->position.y = (float)WINDOWSSYSTEM->GetHeight() - (float)posy;

			temp->w = 100.f;
			temp->h = 100.f;


			renderObj = temp;
			std::cout << "Render component initialising mouse position values\n";
			CPTransform* tran = gameObj->GetComponent<CPTransform>("CPTransform");

			std::cout << tran->GetPosition().x << std::endl;

			gameObj->GetComponent<CPTransform>("CPTransform")->SetPosition(Vec2<float>(4.9f, 24.94f));

			std::cout << tran->GetPosition().x << std::endl;

		}

		void Update() override
		{
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