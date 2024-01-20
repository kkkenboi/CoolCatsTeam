/*!************************************************************************
 \file				GameObjectManager.h
 \author(s)			Kenji Brannon Chong, Ang Jiawei Jarrett
 \par DP email(s):	kenjibrannon.c@digipen.edu, a.jiaweijarrett@digipen.edu
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
#include "LitterBox/Serialization/Serializer.h"
#include <initializer_list>
#include <type_traits>

// For CPP Game Logic
class CPPBehaviour;

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
		 Empty by design
		*************************************************************************/
		~GameObject();

		/*!***********************************************************************
		 \brief
		 Destroys a GameObject and all of its components
		*************************************************************************/
		void Destroy();

		/*!***********************************************************************
		 \brief
		 Gets a specified component within the GameObject
		*************************************************************************/
		template <typename T>
		T* GetComponent() 
		{ 
			// Get an IComponent
			if (std::is_base_of<IComponent, T>::value)
			{
				return static_cast<T*>(m_Components.find(T::GetType())->second);
			}
			else if (std::is_base_of<CPPBehaviour, T>::value)
			{
				//return static_cast<T*>(m_Components[C_CPScriptCPP].)
				return static_cast<T*>(m_Components.find(T::GetType())->second);
			}

			DebuggerAssertFormat(std::is_base_of<IComponent, T>::value, "Tried to get invalid component of type %s", typeid(T).name());

		}


		/*!***********************************************************************
		 \brief
		 Gets all the components of the GameObject
		*************************************************************************/
		std::unordered_multimap<ComponentTypeID, IComponent*> GetAllComponents();

		/*!***********************************************************************
		 \brief
		 Returns true if the GameObject contains the component T
		*************************************************************************/
		template <typename T>
		bool HasComponent()
		{
			DebuggerAssertFormat(std::is_base_of<IComponent, T>::value, "Tried to check invalid component of type %s", typeid(T).name());

			return m_Components.contains(T::GetType());
		}

		/*!***********************************************************************
		 \brief
		 Sets all of the components of one GameObject to another map
		*************************************************************************/
		void SetComponents(const std::unordered_multimap<ComponentTypeID, IComponent*>& );

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

		/*!***********************************************************************
		 \brief
		 Returns the name of this GameObject
		*************************************************************************/
		std::string const& GetName();

		/*!***********************************************************************
		 \brief
		 Sets the name of this GameObject
		*************************************************************************/
		void SetName(std::string const& newName);

		/*!***********************************************************************
		 \brief
		 Returns true if the GameObject is active (enabled)
		*************************************************************************/
		bool IsActive();

		/*!***********************************************************************
		 \brief
		 Sets the state of the GameObject (active or inactive)
		*************************************************************************/
		void SetActive(bool active);

	private:
		std::string											m_name{ "Game Object" };

		std::unordered_multimap<ComponentTypeID, IComponent*>	m_Components;
		int														m_ID;
		bool													m_active;
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
		 Gets all of the current GameObjects in the loaded Scene
		*************************************************************************/
		std::vector<GameObject*> const& GetGameObjects() const;

		/*!***********************************************************************
		 \brief
		 Adds a GameObject to the current pool of GameObjects for loaded scene
		*************************************************************************/
		void AddGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Removes a GameObject from the current pool of GameObjects for loaded scene
		 and deletes it
		*************************************************************************/
		void RemoveGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Removes a GameObject from the current pool of GameObjects for loaded scene
		 WITHOUT deleting it
		*************************************************************************/
		void DetachGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Is the GameObject pointer given currently in the pool of GameObjects for
		 this scene?
		*************************************************************************/
		bool IsGameObjectInScene(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Destroys all of the GameObjects
		*************************************************************************/
		void DestroyAllGOs();

		/*!***********************************************************************
		 \brief	 
		 Gets all of the free-floating GameObjects not bounded to any scene
		*************************************************************************/
		std::vector<GameObject*> const& GetDDOLGameObjects() const;

		/*!***********************************************************************
		 \brief
		 Adds a GameObject to the free-floating pool
		*************************************************************************/
		void AddDDOLGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Removes a GameObject from the free-floating pool
		*************************************************************************/
		void RemoveDDOLGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief
		 Destroys all the free-floating GameObjects not bounded to by any scene
		*************************************************************************/
		void DestroyAllDDOLGOs();

		// Let any subscribers know that a GO is destroyed
		Event<GameObject*> onGameObjectDestroy;

	private:
		// All game objects in the current loaded scene are added to this vector
		std::vector<GameObject*> m_GameObjects;
		// All free-floating game objects (not bounded to a scene) are added to this vector
		std::vector<GameObject*> m_DDOLGameObjects; 
	};

	extern GameObjectManager* GOMANAGER;
}

