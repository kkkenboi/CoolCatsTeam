#pragma once
#include <Litterbox/Engine/Message.h>

namespace LB
{
	// --can be changed to be serialised--
	enum ComponentTypeID
	{
		Component_None = 0,
		Component_Physics,
		Component_Transform,
		Component_Render
	};

	// Interface for derived components to use as a base
	class IComponent
	{
	public:
		IComponent();

		virtual void Initialise() = 0;
		virtual void Serialise() = 0;
		virtual void Update() = 0;

		// To destruct all other derived components
		virtual ~IComponent() {};
	private:
		// To understand what type does this component belong to
		ComponentTypeID TypeID;

	};

	// Interface to make components and tag IDs
	class ComponentMaker
	{
	public:
		ComponentMaker(ComponentTypeID typeId) : TypeID(typeId) {}

		ComponentTypeID TypeID;
		///Create the component
		virtual IComponent* Create();
		virtual ~ComponentMaker() = 0;

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

#define CreateComponentMaker(ComponentType) FACTORY->InitCM ( #ComponentType, new ComponentMakerType<ComponentType>( Component_##ComponentType ) );