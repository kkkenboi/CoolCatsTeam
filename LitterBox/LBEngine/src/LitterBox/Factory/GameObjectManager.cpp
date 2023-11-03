/*!************************************************************************
 \file				GameObjectManager.cpp
 \author(s)			Kenji Brannon Chong | Amadeus Chia Jinhan
 \par DP email(s):	kenjibrannon.c@digipen.edu | amadeusjinhan.chia@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions definition of the GameObject and GameObjectManager 
 class. The GameObject acts as a container to hold all of the different components 
 for the components to interact with each other during the gameloop.

 The GameObjectManager manages all of the GameObject's lifetime throughout the
 entire application.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "GameObjectManager.h"
#include "Components.h"
#include "GameObjectFactory.h"
#include <algorithm>			// For std::find

namespace LB
{
	/***************************************************************************************************
	*
	* Game Object
	*
	***************************************************************************************************/
	
	/*!***********************************************************************
	 \brief
	 Creates a GameObject
	*************************************************************************/
	GameObject::GameObject() : m_Components{}, m_IsActive{ false }, m_ID{} {}

	/*!***********************************************************************
	 \brief
	 Creates a GameObject with an ID
	*************************************************************************/
	GameObject::GameObject(int ID) : m_Components{}, m_IsActive{ false }, m_ID{ ID } {}

	/*!***********************************************************************
	 \brief
	 Destroys a GameObject
	*************************************************************************/
	GameObject::~GameObject() {}

	void GameObject::Destroy()
	{
		//std::cout << "Deleted " << m_name << " Children count " << GetComponent<CPTransform>()->GetChildCount() << "\n";
		//// Delete children GO first if any
		//for (int index{ GetComponent<CPTransform>()->GetChildCount() - 1 }; index >= 0; --index)
		//{
		//	GetComponent<CPTransform>()->GetChild(index)->gameObj->Destroy();
		//}

		// Remove itself from the parent
		if (GetComponent<CPTransform>()->GetParent())
			GetComponent<CPTransform>()->GetParent()->RemoveChild(GetComponent<CPTransform>());

		// Free memory allocated for components
		for (auto const& component : m_Components)
		{
			component.second->Destroy();
			delete component.second;
		}

		delete this;
		//DebuggerLogFormat("GO \"%s\" Deleted", m_name.c_str());
	}

	/*!***********************************************************************
	 \brief
	 Gets all the components of the GameObject

	 \return
	 Map to the GameObject's components
	*************************************************************************/
	std::unordered_map<ComponentTypeID, IComponent*> GameObject::GetComponents()
	{
		return m_Components;
	}

	/*!***********************************************************************
	 \brief
	 Sets all of the components of one GameObject to another map

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::SetComponents(const std::unordered_map<ComponentTypeID, IComponent*>& otherMap)
	{
		this->m_Components = otherMap;
	}

	/*!***********************************************************************
	 \brief
	 Adds a component to the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::AddComponent(ComponentTypeID id, IComponent* component)
	{
		component->gameObj = this;
		m_Components[id] = component;
	}

	/*!***********************************************************************
	 \brief
	 Removes a component from the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::RemoveComponent(ComponentTypeID id)
	{
		m_Components[id]->Destroy();
		m_Components.erase(id);
	}
	
	/*!***********************************************************************
	 \brief
	 Serializes the GameObject data into a file

	 \return
	 True if serialized, else false
	*************************************************************************/
	bool GameObject::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value gameObjName(m_name.c_str(), alloc);
		data.AddMember("Name", gameObjName, alloc);

		if (m_Components.find(C_CPTransform) != m_Components.end())
		{
			//data.SetObject();
			Value TransformComponent;
			m_Components[C_CPTransform]->Serialize(TransformComponent, alloc);
			data.AddMember("Transform", TransformComponent, alloc);
		}
		//We will return false if we fail to serialise a transform because
		//ALL GAMEOBJECTS MUST HAVE TRANSFORM!
		else return false;
		if (m_Components.find(C_CPRigidBody) != m_Components.end())
		{
			Value RigidBodyComponent;
			m_Components[C_CPRigidBody]->Serialize(RigidBodyComponent, alloc);
			data.AddMember("RigidBody", RigidBodyComponent, alloc);
		}
		if (m_Components.find(C_CPRender) != m_Components.end())
		{
			Value RenderComponent;
			m_Components[C_CPRender]->Serialize(RenderComponent, alloc);
			data.AddMember("Render", RenderComponent, alloc);
		}
		if (m_Components.find(C_CPCollider) != m_Components.end())
		{
			Value ColliderComponent;
			m_Components[C_CPCollider]->Serialize(ColliderComponent, alloc);
			data.AddMember("Collider", ColliderComponent, alloc);
		}
		if (m_Components.find(C_CPScriptCPP) != m_Components.end())
		{
			Value CPPScriptComponent;
			m_Components[C_CPScriptCPP]->Serialize(CPPScriptComponent, alloc);
			data.AddMember("CPPScript", CPPScriptComponent, alloc);
		}
		return true;
	}

	/*!***********************************************************************
	 \brief
	 Deserializes the GameObject data from a file

	 \return
	 True if deserialised, else false
	*************************************************************************/
	bool GameObject::Deserialize(const Value& data)
	{
		bool HasName = data.HasMember("Name");
		bool HasTransform = data.HasMember("Transform");
		bool HasRigidBody = data.HasMember("RigidBody");
		bool HasRender = data.HasMember("Render");
		bool HasCollider = data.HasMember("Collider");
		bool HasCPPScript = data.HasMember("CPPScript");
		if (data.IsObject())
		{
			if (HasName)
			{
				const Value& nameValue = data["Name"];
				m_name = nameValue.GetString();
			}
			if (HasTransform)
			{
				if (m_Components.find(C_CPTransform) == m_Components.end())
				{
					DebuggerLog("Deserialize: GO doesn't have a transform :C so we make one");
					AddComponent(C_CPTransform, FACTORY->GetCMs()[C_CPTransform]->Create());
				}
				const Value& transformValue = data["Transform"];
				m_Components[C_CPTransform]->Deserialize(transformValue);
			}
			//ALL GO's MUST HAVE TRANSFORM!
			else return false;
			if (HasRigidBody)
			{
				if (m_Components.find(C_CPRigidBody) == m_Components.end())
				{
					DebuggerLog("Deserialize: GO doesn't have a rigidbody :C so we make one");
					AddComponent(C_CPRigidBody, FACTORY->GetCMs()[C_CPRigidBody]->Create());
				}
				const Value& rigidBodyValue = data["RigidBody"];
				m_Components[C_CPRigidBody]->Deserialize(rigidBodyValue);
			}
			if (HasRender)
			{
				if (m_Components.find(C_CPRender) == m_Components.end())
				{
					DebuggerLog("Deserialize: GO doesn't have a render :C so we make one");
					AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
				}
				const Value& renderValue = data["Render"];
				m_Components[C_CPRender]->Deserialize(renderValue);
			}
			if (HasCPPScript)
			{
				if (m_Components.find(C_CPScriptCPP) == m_Components.end())
				{
					DebuggerLog("Deserialize: GO doesn't have a CPP Script :C so we make one");
					AddComponent(C_CPScriptCPP, FACTORY->GetCMs()[C_CPScriptCPP]->Create());
				}
				const Value& cppScriptValue = data["CPPScript"];
				m_Components[C_CPScriptCPP]->Deserialize(cppScriptValue);
			}
			if (HasCollider)
			{
				if (m_Components.find(C_CPCollider) == m_Components.end())
				{
					DebuggerLog("Deserialize: GO doesn't have a Collider :C so we make one");
					AddComponent(C_CPCollider, FACTORY->GetCMs()[C_CPCollider]->Create());
				}
				const Value& colliderValue = data["Collider"];
				m_Components[C_CPCollider]->Deserialize(colliderValue);
				DebuggerLogFormat("coll size %d", this->GetComponent<CPCollider>()->m_widthUnscaled);
			}
		}
		this->StartComponents();
		return true;
	}

	/*!***********************************************************************
	 \brief
	 Initialises all of the components in the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::StartComponents()
	{
		for (auto const& component : m_Components)
		{
			component.second->Initialise();
		}
	}

	/*!***********************************************************************
	 \brief
	 Gets the ID of the GameObject

	 \return
	 ID of the GameObject
	 *************************************************************************/
	int GameObject::GetID() const
	{
		return m_ID;
	}

	/*!***********************************************************************
	 \brief
	 Sets the ID of the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::SetID(int ID)
	{
		m_ID = ID;
	}

	std::string const& GameObject::GetName()
	{
		return m_name;
	}

	void GameObject::SetName(std::string const& newName)
	{
		m_name = newName;
	}

	/***************************************************************************************************
	*
	* Game Object Manager
	*
	***************************************************************************************************/

	GameObjectManager* GOMANAGER = nullptr;

	/*!***********************************************************************
	 \brief
	 Creates the GameObjectManager
	*************************************************************************/
	GameObjectManager::GameObjectManager()
	{
		SetSystemName("GameObjectManager System");

		if (!GOMANAGER)
			GOMANAGER = this;
		else
			DebuggerLogError("GameObject Manager already exist");
	}

	/*!***********************************************************************
	 \brief
	 Destroys all of the GameObjects and signifies end of lifecycle of
	 GameObjectManager

	 \return
	 Nothing
	*************************************************************************/
	void GameObjectManager::Destroy()
	{
		DestroyAllDDOLGOs();
		DestroyAllGOs();
		DebuggerLog("GameObject Manager destructed");
	}

	/*!***********************************************************************
	 \brief
	 Gets all of the current GameObjects the GameObjectManager is managing

	 \return
	 All current GameObjects
	*************************************************************************/
	std::vector<GameObject*> const& GameObjectManager::GetGameObjects() const
	{
		return m_GameObjects;
	}
	/*!***********************************************************************
	 \brief
	 Adds a GameObject to the current pool of GameObjects

	 \return
	 Nothing
	*************************************************************************/
	void GameObjectManager::AddGameObject(GameObject* gameObject)
	{
		m_GameObjects.push_back(gameObject);
	}

	void GameObjectManager::RemoveGameObject(GameObject* gameObject)
	{
		auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), gameObject);
		if (it != m_GameObjects.end()) 
		{
			m_GameObjects.erase(it);
			gameObject->Destroy();
		}
		else
		{
			DebuggerLogErrorFormat("[GO Manager] Tried to delete invalid GO \"%s\"", gameObject->GetName().c_str());
		}
	}

	/*!***********************************************************************
	 \brief
	 Destroys all of the GameObjects

	 \return
	 Nothing
	*************************************************************************/
	void GameObjectManager::DestroyAllGOs()
	{
		// Destroying gameobjects
		for (int i{ (int)m_GameObjects.size()-1 }; i >= 0; --i)
		{
			RemoveGameObject(m_GameObjects[i]);
		}

		// Set size of game objects to empty
		m_GameObjects.clear();

		DebuggerLog("[GOManager] All GOs deleted");
	}

	std::vector<GameObject*> const& GameObjectManager::GetDDOLGameObjects() const
	{
		return m_DDOLGameObjects;
	}

	void GameObjectManager::AddDDOLGameObject(GameObject* gameObject)
	{
		m_DDOLGameObjects.push_back(gameObject);
	}

	void GameObjectManager::RemoveDDOLGameObject(GameObject* gameObject)
	{
		auto it = std::find(m_DDOLGameObjects.begin(), m_DDOLGameObjects.end(), gameObject);
		if (it != m_DDOLGameObjects.end())
		{
			m_DDOLGameObjects.erase(it);
			gameObject->Destroy();
		}
		else
		{
			DebuggerLogErrorFormat("[GO Manager] Tried to delete invalid DDOL GO \"%s\"", gameObject->GetName().c_str());
		}
	}

	void GameObjectManager::DestroyAllDDOLGOs()
	{
		// Destroying gameobjects
		for (int i{ (int)m_DDOLGameObjects.size() - 1 }; i >= 0; --i)
		{
			RemoveDDOLGameObject(m_DDOLGameObjects[i]);
		}

		// Set size of game objects to empty
		m_DDOLGameObjects.clear();

		DebuggerLog("[GOManager] All DDOL GOs deleted");
	}
}