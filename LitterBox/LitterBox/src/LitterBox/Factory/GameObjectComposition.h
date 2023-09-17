#pragma once
#include "pch.h"
#include <LitterBox/Core/System.h>
#include <LitterBox/Factory/Components.h>

namespace LB
{
	using GOCID = unsigned int;

	class GameObjectComposition
	{
	public:

		// Enables the construction/destruction of GameObjectComposition
		friend class GameObjectFactory;

		///Message interface See Message.h
		///Message will be sent to each component on this composition.
		void SendMessage(Message* message);

		///Get a component on this composition by component's typename. 
		///This will return NULL if the component is not found.
		GameComponent* GetComponent(ComponentTypeID typeId);

		///Type safe way of accessing components.
		template<typename type>
		type* GetComponentType(ComponentTypeID typeId);

		///Initialize all the components on the composition.
		///See function for details.
		void Initialize();

		///Correctly destroys an object by adding it to the factories destroy list.
		///Object will be destroy the next time the factory updates.
		void Destroy();

		///Add an component used only for dynamic composition construction.
		void AddComponent(ComponentTypeID typeId, GameComponent* component);

		///Get the game object's Id
		GOCID GetID() { return GameObjID; }

	private:

		std::vector<GameComponent*> Components;
		using ComponentIt = std::vector<GameComponent*>::iterator;

		GOCID GameObjID;

		// Only accessed by the GameObjectFactory through CreateEmptyComposition
		GameObjectComposition();
		~GameObjectComposition();

	};
}

