#pragma once
#include "pch.h"
#include "Components.h"
#include "GameObjectManager.h"
#include "LitterBox/Core/System.h"

namespace LB
{
	// This class creates ComponentMakers to create Components 
	class FactorySystem : public ISystem
	{
	public:
		// Let the factory initialize the known component types listed in the file
		// and/or start creating game objects from a data file
		FactorySystem();

		// Based on event/messaging system, how many game objects to be created/destroyed
		// will be updated here
		void Update() override;

		// Communicate with other systems
		void SendMessage(Message* message) override;

		std::string GetName() override { return "Factory"; }

		// Serialise all known ComponentMakers
		void SerialiseGameObjs(int json_thing);

		// Future Feature?
		// void AddCM();

		// Create all known ComponentMaker of their type, eg. Physics / Graphics
		void InitCM(const std::string& name, ComponentMaker* newComponent);

		// Since the 
		void DeleteAllCMs(std::map<std::string, ComponentMaker*> ComponentMakers);

		// Creates a game object with a component list, if component list is empty, no components
		// will be tied to the game object
		GameObject* CreateGameObject(std::vector<IComponent*> componentsList = std::vector<IComponent*>());

		void AddComponent(GameObject* gameObj, IComponent* component);

		std::map<std::string, ComponentMaker*> GetCMs() const;

		~FactorySystem() override;
	private:
		// Needs to hold all of the componentmakers to know what available componentmakers are there
		// Does not need to have the game objects because that is the gameobjmanager job to hold
		// Strictly only to create game objects and let go from there on

		// For now we use map to hold the componentmakers, might change in the future
		std::map<std::string, ComponentMaker*> m_ComponentMakers;

		std::vector<GameObject*> m_WaitingList;

		bool toUpdate = false;

	};

	extern FactorySystem* FACTORY;
}

