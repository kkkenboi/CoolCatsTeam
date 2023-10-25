/*!************************************************************************
 \file				GameObjectManager.h
 \author(s)			Kenji Brannon Chong | Amadeus Chia Jinhan
 \par DP email(s):	kenjibrannon.c@digipen.edu | amadeusjinhan.chia@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief		

 This file contains functions declarations and definition of the GameObject 
 and GameObjectManager class. The GameObject acts as a container to hold all 
 of the different components for the components to interact with each other 
 during the gameloop.

 The GameObjectManager manages all of the GameObject's lifetime throughout the 
 entire application.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "pch.h"
#include "LitterBox/Core/System.h"
#include <initializer_list>
#include "LitterBox/Serialization/Serializer.h"
#include <type_traits>

namespace LB
{
	class IComponent; // Forward Declaration
	enum ComponentTypeID : int;

	/*!***********************************************************************
	 \brief
	 GameObject class contains components that contains different functionalities
	 and will be updated by their own systems.
	*************************************************************************/
	class GameObject
	{
	public:
		/*!***********************************************************************
		 \brief
		 Creates a GameObject
		*************************************************************************/
		GameObject();

		/*!***********************************************************************
		 \brief
		 Creates a GameObject with an ID
		*************************************************************************/
		GameObject(int ID);

		/*!***********************************************************************
		 \brief
		 Destroys a GameObject
		*************************************************************************/
		~GameObject();

		/*!***********************************************************************
		 \brief
		 Gets a specified component within the GameObject
		*************************************************************************/
		/*template <typename T>
		T* GetComponent(std::string name) { return static_cast<T*>(m_Components[name]); }*/

		template <typename T>
		T* GetComponent() 
		{ 
			DebuggerAssertFormat(std::is_base_of<IComponent, T>::value, "Tried to get invalid component of type %s", typeid(T).name());

			return static_cast<T*>(m_Components[T().GetType()]);
		}

		template <typename T>
		bool HasComponent()
		{
			DebuggerAssertFormat(std::is_base_of<IComponent, T>::value, "Tried to check invalid component of type %s", typeid(T).name());

			return m_Components.find(T().GetType()) != m_Components.end();
		}

		/*!***********************************************************************
		 \brief
		 Gets all the components of the GameObject
		*************************************************************************/
		std::unordered_map<ComponentTypeID, IComponent*> GetComponents();

		/*!***********************************************************************
		 \brief
		 Sets all of the components of one GameObject to another map
		*************************************************************************/
		void SetComponents(const std::unordered_map<ComponentTypeID, IComponent*>& );

		/*!***********************************************************************
		 \brief
		 Adds a component to the GameObject
		*************************************************************************/
		void AddComponent(ComponentTypeID id, IComponent* component);

		/*!***********************************************************************
		 \brief
		 Removes a component from the GameObject
		*************************************************************************/
		void RemoveComponent(ComponentTypeID id);

		/*!***********************************************************************
		 \brief
		 Serializes the GameObject data into a file
		*************************************************************************/
		bool Serialize(Value&, Document::AllocatorType&);

		/*!***********************************************************************
		 \brief
		 Deserializes the GameObject data from a file
		*************************************************************************/
		bool Deserialize(const Value&);

		/*!***********************************************************************
		 \brief
		 Initialises all of the components in the GameObject
		*************************************************************************/
		void StartComponents();

		/*!***********************************************************************
		 \brief
		 Gets the ID of the GameObject
		 *************************************************************************/
		int GetID() const;

		/*!***********************************************************************
		 \brief
		 Sets the ID of the GameObject
		*************************************************************************/
		void SetID(int ID);

		std::string const& GetName();

		void SetName(std::string const& newName);

	private:
		std::string											m_name{ "Game Object" };

		std::unordered_map<ComponentTypeID, IComponent*>	m_Components;
		int													m_ID;
		bool												m_IsActive;
	};

	/*!***********************************************************************
	 \brief
	 GameObjectManager class is a system that manages all of the game objects.
	*************************************************************************/
	class GameObjectManager : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Creates the GameObjectManager
		*************************************************************************/
		GameObjectManager();

		/*!***********************************************************************
		 \brief
		 Destroys all of the GameObjects and signifies end of lifecycle of
		 GameObjectManager
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief
		 Gets all of the current GameObjects the GameObjectManager is managing
		*************************************************************************/
		std::vector<GameObject*> GetGameObjects() const;

		/*!***********************************************************************
		 \brief
		 Adds a GameObject to the current pool of GameObjects
		*************************************************************************/
		void AddGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Destroys all of the GameObjects
		*************************************************************************/
		void DestroyAllGOs();

	private:
		std::vector<GameObject*> m_GameObjects;
	};

	extern GameObjectManager* GOMANAGER;
}

