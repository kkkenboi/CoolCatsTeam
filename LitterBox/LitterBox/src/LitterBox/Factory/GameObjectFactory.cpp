#include "GameObjectFactory.h"
#include "Components.cpp"
#include "GameObjectManager.h"

namespace LB
{
	Factory* FACTORY = nullptr;

	Factory::Factory()
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


		// Creating a empty game object
		std::vector<IComponent*> empty;
		CreateGameObject(empty);

		// Input test components
		std::vector<IComponent*> notEmpty;
		notEmpty.push_back(m_ComponentMakers["Physics"]->Create());
		notEmpty.push_back(m_ComponentMakers["Transform"]->Create());
		notEmpty.push_back(m_ComponentMakers["Render"]->Create());
		CreateGameObject(notEmpty);


	}

	void Factory::SerialiseGameObjs(int jsonThing)
	{
		// Serialise the Game Objects with all of their data
		//
		jsonThing = 1;
		// Update the Factory 
		// if more than one game object created, set toUpdate to true
	}

	void Factory::InitCM(const std::string& name, ComponentMaker* newComponent)
	{
		m_ComponentMakers[name] = newComponent;
	}

	void Factory::DeleteAllCMs(std::map<std::string, ComponentMaker*> ComponentMakers)
	{
		for (std::map<std::string, ComponentMaker*>::iterator it = ComponentMakers.begin(); it != ComponentMakers.end(); ++it)
		{
			delete it->second;
		}

		std::cout << "ComponentMakers all deleted\n";
	}

	void Factory::Update(float deltaTime)
	{
		deltaTime;

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

	void Factory::SendMessage(Message* message) 
	{ 
		UNREFERENCED_PARAMETER(message); 
	};


	void Factory::CreateGameObject(std::vector<IComponent*> componentsList)
	{
		GameObject obj(componentsList);

		m_WaitingList.push_back(obj);

		std::cout << "Game Object pushed back into waiting list\n";

		toUpdate = true;
	}


	Factory::~Factory()
	{
		DeleteAllCMs(m_ComponentMakers);

		for (size_t i{}; i < m_WaitingList.size(); ++i)
		{
			for (size_t j{}; j < m_WaitingList[i].m_Components.size(); ++j)
			{
				delete m_WaitingList[i].m_Components[j];
				std::cout << "One Game Object component deleted from game object " << i << "\n";
			}

			std::cout << "Game Object " << i << " has been deleted\n";

			if (i + 1 == m_WaitingList.size())
			{
				std::cout << "Game Object components all deleted\n";
			}
		}
	}
}