#pragma once
#include <Litterbox/Engine/Message.h>
#include <Litterbox/Factory/GameObjectComposition.h>

namespace LB
{
	// can be changed to be serialised
	enum ComponentTypeID
	{
		Component_None = 0,
		Component_Physics,
		Component_Transform,
		Component_Render
	};

	class ComponentCreator
	{
	public:
		ComponentCreator(ComponentTypeID typeId)
			:TypeId(typeId)
		{
		}

		ComponentTypeID TypeId;
		///Create the component
		virtual ~ComponentCreator() {}

	};

	// To use RegisterComponent to test things out quickly,
	// Define found at the bottom of file
	template<typename type>
	class ComponentCreatorType : public ComponentCreator
	{
	public:
		ComponentCreatorType(ComponentTypeID typeId)
			:ComponentCreator(typeId)
		{
		}

		GameComponent* Create()
		{
			return new type();
		}
	};

	class GameComponent
	{
	public:
		// To access 
		friend class GameObjectComposition;

		///Signal that the component is now active in the game world.
		///See GameObjectComposition for details.
		virtual void Initialize() {};

		///GameComponents receive all messages send to their owning composition. 
		///See Message.h for details.
		virtual void SendMessage(Message*) {};

		// -- Will be using our serializer --
		/////Component Serialization Interface see Serialization.h for details.
		//virtual void Serialize(ISerializer& str) { UNREFERENCED_PARAMETER(str); };
		// ----------------------------------

		///Get the GameObjectComposition this component is owned/composed.
		GameObjectComposition* GetOwner() { return Base; }

		ComponentTypeID TypeID;

	protected:
		///Destroy the component.
		virtual ~GameComponent() {};

	private:
		///Each component has a pointer back to the base owning composition.
		GameObjectComposition* Base;

	};

}

#define RegisterComponent(type) FACTORY->AddComponentCreator( #type, new ComponentCreatorType<type>( CT_##type ) );