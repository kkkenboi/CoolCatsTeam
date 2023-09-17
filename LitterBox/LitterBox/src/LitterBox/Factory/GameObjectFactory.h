#pragma once
#include "pch.h"
#include <LitterBox/Core/System.h>
#include <LitterBox/Factory/Components.h>
#include <LitterBox/Factory/GameObjectComposition.h>

namespace LB
{
	class GameObjectFactory : public ISystem
	{
	public:

		GameObjectFactory();
		~GameObjectFactory();

		///Create initialize and Id a GOC from the data file.
		GameObjectComposition* Create(const std::string& filename);

		///Add a GOC to the destroy list for delayed destruction.
		void Destroy(GameObjectComposition* gameObject);

		///Update the factory, destroying dead objects.
		virtual void Update(float dt) override;

		///Name of the system is factory.
		virtual std::string GetName() override { return "Factory"; }

		///Message Interface see Message.h
		virtual void SendMessage(Message* message) override;

		///Destroy all the GOCs in the world. Used for final shutdown.
		void DestroyAllObjects();

		///Create and Id a GOC at runtime. Used to dynamically build GOC.
		///After components have been added call GOC->Initialize().
		GameObjectComposition* CreateEmptyComposition();

		///Build a composition and serialize from the data file but do not initialize the GOC.
		///Used to create a composition and then adjust its data before initialization
		///see GameObjectComposition::Initialize for details.
		GameObjectComposition* BuildAndSerialize(const std::string& filename);

		///Id object and store it in the object map.
		void IDGameObject(GameObjectComposition* gameObject);

		///Add a component creator enabling data driven composition
		void AddComponentCreator(const std::string& name, ComponentCreator* creator);

		///Get the game object with given id. This function will return NULL if
		///the object has been destroyed.
		GameObjectComposition* GetObjectWithID(GOCID id);

	private:
		///Used to incrementally generate unique id's.
		unsigned LastGameObjectID;

		///Map of component creator used for data driven composition
		typedef std::map<std::string, ComponentCreator*> ComponentMapType;
		ComponentMapType ComponentMap;

		///Map of GOC to their Ids used for safe referencing of game objects
		typedef std::map<unsigned, GameObjectComposition*> GameObjectIDMapType;
		GameObjectIDMapType GameObjectIDMap;

		///Objects to be deleted
		///this is a set to prevent problems when a game object
		///is deleted in multiple locations
		std::set<GameObjectComposition*> ObjectsToBeDeleted;

	};

	extern GameObjectFactory* FACTORY;
}

