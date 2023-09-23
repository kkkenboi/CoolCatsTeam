#include "GameObjectFactory.h"
#include "Components.cpp"
#include "GameObjectManager.h"

namespace LB
{
	FactorySystem* FACTORY = nullptr;

	FactorySystem::FactorySystem()
	{
		// Ensure singleton
		if (!FACTORY)
		{
			FACTORY = this;
		}
		else
		{
			std::cerr << "Factory already exist\n";
		}

		// Deserialise the data file and initialise ComponentMakers
		// 
		CreateComponentMaker(Physics);
		CreateComponentMaker(Transform);
		CreateComponentMaker(Render);

		// Game Object Data File
		// Deserialise the data file and initialise the game objects with it
		SerialiseGameObjs(1);

		std::cout << "Factory Initialised\n";


		//// Creating a empty game object
		//std::vector<IComponent*> empty;
		//CreateGameObject(empty);

		//// Input test components
		//std::vector<IComponent*> notEmpty;
		//notEmpty.push_back(m_ComponentMakers["Physics"]->Create());
		//notEmpty.push_back(m_ComponentMakers["Transform"]->Create());
		//notEmpty.push_back(m_ComponentMakers["Render"]->Create());
		//CreateGameObject(notEmpty);

	}

	void FactorySystem::SerialiseGameObjs(int jsonThing)
	{
		// Serialise the Game Objects with all of their data
		//
		jsonThing = 1;
		// Update the Factory 
		// if more than one game object created, set toUpdate to true
	}

	void FactorySystem::InitCM(const std::string& name, ComponentMaker* newComponent)
	{
		m_ComponentMakers[name] = newComponent;
	}

	void FactorySystem::DeleteAllCMs(std::map<std::string, ComponentMaker*> ComponentMakers)
	{
		for (std::map<std::string, ComponentMaker*>::iterator it = ComponentMakers.begin(); it != ComponentMakers.end(); ++it)
		{
			delete it->second;
		}

		std::cout << "ComponentMakers all deleted\n";
	}

	void FactorySystem::Update()
	{

		if (toUpdate)
		{
			// Send all of the game objects to the GameObjectManager's vector or something
			// Send message to the GameObjectManager to add objects to its pool of game objects
			// What about pools?

			// For now just make it be the same
			//GOMANAGER->m_Pool = m_WaitingList;
			std::cout << "Factory Updated\n";
		}

		toUpdate = false;
	}

	void FactorySystem::SendMessage(Message* message)
	{ 
		UNREFERENCED_PARAMETER(message); 
	};


	GameObject* FactorySystem::CreateGameObject(std::vector<IComponent*> componentsList)
	{
		std::cout << "Game Object pushed back into waiting list\n";

		//toUpdate = true;

		return new GameObject(componentsList);
	}

	void FactorySystem::AddComponent(GameObject* gameObj, IComponent* component)
	{
		if (gameObj != nullptr)
		{
			gameObj->m_Components.push_back(component);
		}
	}

	std::map<std::string, ComponentMaker*> FactorySystem::GetCMs() const
	{
		return m_ComponentMakers;
	}


	void FactorySystem::Destroy()
	{
		DeleteAllCMs(m_ComponentMakers);
	}
}