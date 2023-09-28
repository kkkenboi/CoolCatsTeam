/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

**************************************************************************/

#include "GameObjectManager.h"
#include "Components.h"
#include "GameObjectFactory.h"
namespace LB
{
	/***************************************************************************************************
	*
	* Game Object Manager
	*
	***************************************************************************************************/

	GameObjectManager* GOMANAGER = nullptr;

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObjectManager::GameObjectManager()
	{
		SetSystemName("GameObjectManager System");

		if (!GOMANAGER)
		{
			GOMANAGER = this;
		}
		else
		{
			std::cerr << "GameObjectManager already exist\n";
		}
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	std::vector<GameObject*> GameObjectManager::GetGameObjects() const
	{
		return m_GameObjects;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void GameObjectManager::AddGameObject(GameObject* gameObject)
	{
		m_GameObjects.push_back(gameObject);
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void GameObjectManager::DestroyAllGOs()
	{
		std::cout << "Entering!!\n";
		// Destroying components in game objects
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			//int componentSize = m_GameObjects[i]->GetComponents().size();
			for (auto const& component : m_GameObjects[i]->GetComponents())
			{
				component.second->Destroy();
				// Delete the memory allocated for the component
				delete component.second;

				//// --Change this to check the delete for which component--
				std::cout << "One GO component deleted from game object " << m_GameObjects[i]->GetID() << "\n";
			}


			std::cout << "GO " << m_GameObjects[i]->GetID() << " has been deleted\n";

			if (i + 1 == m_GameObjects.size())
			{
				std::cout << "GO's components all deleted\n";
			}
		}

		// Destroying gameobjects
		size_t gameObjSize = m_GameObjects.size();
		for (size_t i{}; i < gameObjSize; ++i)
		{
			delete m_GameObjects[i];
		}

		// Set size of game objects to empty
		m_GameObjects.clear();

		std::cout << m_GameObjects.size() << std::endl;

		std::cout << "All GOs deleted\n";
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void GameObjectManager::Destroy()
	{
		DestroyAllGOs();
		std::cout << "GOM destructed\n";
	}

	/***************************************************************************************************
	*
	* Game Object
	*
	***************************************************************************************************/
	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObject::GameObject(int ID) : m_Components{}, isActive{ false }, m_ID{ ID }
	{
		std::cout << "GO constructed\n";
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObject::GameObject() : m_Components{}, isActive{ false }, m_ID{}
	{
		std::cout << "GO constructed\n";
	}

	//GameObject::GameObject(std::vector<IComponent*> const& componentList) : m_Components{ componentList }, isActive{ false }
	//{
	//	std::cout << "GO parameterized constructor\n";
	//};

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	GameObject::~GameObject()
	{
		// Should delete all of the components
		std::cout << "GO destructed\n";
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	std::unordered_map<std::string, IComponent*> GameObject::GetComponents()
	{
		return m_Components;
	}

	void GameObject::SetComponents(const std::unordered_map<std::string, IComponent*>& otherMap)
	{
		this->m_Components = otherMap;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void GameObject::AddComponent(std::string name, IComponent* component)
	{
		component->gameObj = this;
		m_Components[name] = component;
	}

	bool GameObject::Serialize(Value& data, Document::AllocatorType& alloc)
	{
	
		if (m_Components.find("CPTransform") != m_Components.end())
		{
			data.SetObject();
			Value TransfromComponent;
			m_Components["CPTransform"]->Serialize(TransfromComponent, alloc);
			data.AddMember("Transform", TransfromComponent, alloc);
		}
		//We will return false if we fail to serialise a transform because
		//ALL GAMEOBJECTS MUST HAVE TRANSFORM!
		else return false;
		if (m_Components.find("CPRigidBody") != m_Components.end())
		{
			Value RigidBodyComponent;
			m_Components["CPRigidBody"]->Serialize(RigidBodyComponent,alloc);
			data.AddMember("RigidBody", RigidBodyComponent, alloc);
		}
		if (m_Components.find("CPRender") != m_Components.end())
		{
			Value RenderComponent;
			m_Components["CPRender"]->Serialize(RenderComponent, alloc);
			data.AddMember("Render", RenderComponent, alloc);
		}
		return true;
	}

	bool GameObject::Deserialize(const Value& data)
	{
		bool HasTransform = data.HasMember("Transform");
		bool HasRigidBody = data.HasMember("RigidBody");
		bool HasRender = data.HasMember("Render");
		if (data.IsObject())
		{
			if (HasTransform)
			{
				if (m_Components.find("CPTransform") == m_Components.end())
				{
					std::cout << "GO doesn't have a transform :C so we make one\n";
					AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
				}
				const Value& transformValue = data["Transform"];
				m_Components["CPTransform"]->Deserialize(transformValue);
			}
			//ALL GO's MUST HAVE TRANSFORM!
			else return false; 
			if (HasRigidBody)
			{
				if (m_Components.find("CPRigidBody") == m_Components.end())
				{
					std::cout << "GO doesn't have a rigidbody :C so we make one\n";
					AddComponent("CPRigidBody", FACTORY->GetCMs()["CPRigidBody"]->Create());
				}
				const Value& rigidBodyValue = data["RigidBody"];
				m_Components["CPRigidBody"]->Deserialize(rigidBodyValue);
			}
			if (HasRender)
			{
				if (m_Components.find("CPRender") == m_Components.end())
				{
					std::cout << "GO doesn't have a render :C so we make one\n";
					AddComponent("CPRender", FACTORY->GetCMs()["CPRender"]->Create());
				}
				const Value& renderValue = data["Render"];
				m_Components["CPRender"]->Deserialize(renderValue);
			}
		}
		return true;
	}

	void GameObject::StartComponents()
	{
		for (auto const& component : m_Components)
		{
			component.second->Initialise();
		}
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	int GameObject::GetID() const
	{
		return m_ID;
	}

	/*!***********************************************************************
	 \brief


	 \return

	*************************************************************************/
	void GameObject::SetID(int ID)
	{
		m_ID = ID;
	}
}