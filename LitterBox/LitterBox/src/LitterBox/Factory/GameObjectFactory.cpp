/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

**************************************************************************/

#include "GameObjectFactory.h"
#include "Components.h"

namespace LB
{
	FactorySystem* FACTORY = nullptr;


	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
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

		m_LastObjID = 0;

		// Deserialise the data file and initialise ComponentMakers
		// 
		CreateComponentMaker(CPTransform);
		CreateComponentMaker(CPRender);
		CreateComponentMaker(CPRigidBody);

		// Game Object Data File
		// Deserialise the data file and initialise the game objects with it
		SerialiseGameObjs(1);

		//GameObject* gameObj = CreateGameObject();
		//if (gameObj->GetID() == 0) 		// ID only starts at 1
		//{
		//	gameObj->SetID(GetLastObjID());
		//}
		////gameObj->AddComponent(FACTORY->GetCMs()["Physics"]->Create());
		//gameObj->AddComponent("CPTransform", GetCMs()["CPTransform"]->Create());
		//gameObj->AddComponent("CPRender", GetCMs()["CPRender"]->Create());
		////gameObj->GetComponents()[0]->Initialise();
		//gameObj->StartComponents();


		std::cout << "Factory Initialised\n";
	}


	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void FactorySystem::SerialiseGameObjs(int jsonThing)
	{
		// Serialise the Game Objects with all of their data
		//
		jsonThing = 1;
		// Update the Factory 
		// if more than one game object created, set toUpdate to true
	}


	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void FactorySystem::InitCM(const std::string& name, ComponentMaker* newComponent)
	{
		m_ComponentMakers[name] = newComponent;
	}


	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void FactorySystem::DeleteAllCMs(std::map<std::string, ComponentMaker*> ComponentMakers)
	{
		for (std::map<std::string, ComponentMaker*>::iterator it = ComponentMakers.begin(); it != ComponentMakers.end(); ++it)
		{
			delete it->second;
		}

		std::cout << "ComponentMakers all deleted\n";
	}


	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
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


	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void FactorySystem::SendMessage(Message* message)
	{ 
		UNREFERENCED_PARAMETER(message); 
	};

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObject* FactorySystem::CreateGameObject()
	{
		++m_LastObjID;
		std::cout << "GO " << m_LastObjID << " has been created\n";
		//toUpdate = true;

		// Does this mean that only default constructors are allowed?
		//return MEMORY->Allocate<GameObject>();

		// Original return
		return new GameObject(FACTORY->GetLastObjID());
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	std::map<std::string, ComponentMaker*> FactorySystem::GetCMs() const
	{
		return m_ComponentMakers;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	int FactorySystem::GetLastObjID() const
	{
		return m_LastObjID;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(Vec2<float> pos)
	{
		return SpawnGameObject({}, pos);
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(std::initializer_list<std::string> components, Vec2<float> pos)
	{
		// Creating the game object
		GameObject* gameObj = FACTORY->CreateGameObject();
		if (gameObj->GetID() == 0) 		// ID only starts at 1
		{
			gameObj->SetID(FACTORY->GetLastObjID());
		}

		// Every gameobject must have a transform!!!
		gameObj->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
		gameObj->GetComponent<CPTransform>("CPTransform")->SetPosition(pos);

		for (std::string component : components)
		{
			gameObj->AddComponent(component, FACTORY->GetCMs()[component]->Create());
		}
		gameObj->StartComponents();

		// Sends game object to the Game Object Manager
		// For now, push back in the function to go manager,
		// However, in the future we might need to change when we need to render a lot in one go
		// or send a equal amount of game objects at one go
		// Might be redundant too because we should initialize a pool at the start
		GOMANAGER->AddGameObject(gameObj);
		return gameObj;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void FactorySystem::Destroy()
	{
		DeleteAllCMs(m_ComponentMakers);
	}
}