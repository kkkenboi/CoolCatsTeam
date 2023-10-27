/*!************************************************************************
 \file                Scene.h
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
  This file contains the functions for the Scene. A scene can be thought of
  as the root game object for that holds every other game object. It is 
  then managed by the Scene Manager

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "LitterBox/Components/TransformComponent.h"
#include "LitterBox/Serialization/Serializer.h"
#include "LitterBox/Factory/GameObjectFactory.h"


namespace LB
{
	/*!***********************************************************************
	 \brief
	 Scene class that holds data needed for a scene
	*************************************************************************/
	class Scene
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initialises the Scene
		*************************************************************************/
		void Init() 
		{ 
			JSONSerializer::DeserializeFromFile("Scenetest.json", *this);
		}

		/*!***********************************************************************
		 \brief
		 Update function for the scene
		*************************************************************************/
		virtual void Update() {}

		/*!***********************************************************************
		 \brief
		 Destroy function that handles the destruction of the scene
		*************************************************************************/
		void Destroy() 
		{
			// To do: Serialize on save key press and not in play mode

			JSONSerializer::SerializeToFile("Scenetest.json", *this);
		}

		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			data.SetObject();
			Value rootObject;
			root.Serialize(rootObject, alloc);
			data.AddMember("Root", rootObject, alloc);
			
			return true;
		}

		bool Deserialize(const Value& data)
		{
			bool HasRoot = data.HasMember("Root");
			if (data.IsObject())
			{
				if (HasRoot)
				{
					const Value& rootValue = data["Root"];
					root.Deserialize(rootValue);
					return true;
				}
			}
			return false;
		}

		std::string GetName()
		{
			return m_name;
		}

	protected:
		std::string m_name{ "Scene" };
		CPTransform root{};	// All game objects are children of this root game object
	};
}
