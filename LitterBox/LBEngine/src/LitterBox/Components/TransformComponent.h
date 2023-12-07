/*!************************************************************************
 \file				TransformComponent.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief
 This file contains functions of the CPTransform class.

 CPTransform is a component that stores the position, scale, and rotation
 of the GameObject in the game world. If the other components want to move/
 scale/etc the gameobject, they will interface with this component.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "Component.h"
#include "LitterBox/Utils/Math.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 CPTransform is a default component that every game object will have. It
	 contains a position vector, scale vector and rotation vector.
	*************************************************************************/
	class CPTransform : public IComponent
	{
	public:
		/*!***********************************************************************
		 \brief
		 Returns this component type
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPTransform;
		}

		/*!***********************************************************************
		 \brief
		 Function will serialize data from an outer json file to get data.
		 \return
		 returns true if serialization succeeds and false if not.
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{
			DebuggerLog("Serializing Transform");
			data.SetObject();
			Value PositionValue;
			if (m_pos.Serialize(PositionValue, alloc))
			{
				data.AddMember("Position", PositionValue, alloc);
			}
			else return false;
			Value ScaleValue;
			if (m_scale.Serialize(ScaleValue, alloc))
			{
				data.AddMember("Scale", ScaleValue, alloc);
			}
			else return false;
			data.AddMember("Rotation", m_angle, alloc);
			if (!m_children.empty())
			{
				Value childArray(rapidjson::kArrayType);
				for (const auto& child : m_children)
				{
					Value childValue;
					if (child->gameObj->Serialize(childValue,alloc))
					{
						childArray.PushBack(childValue, alloc);
					}
				}
				data.AddMember("Children", childArray, alloc);
			}
			return true;
		}

		/*!***********************************************************************
		\brief
		 Function will save data from scene to json file.

		\return
		 returns true if deserialization succeeds and false if not.
		*************************************************************************/
		bool Deserialize(const Value& data) override;

		/*!***********************************************************************
		\brief
		 Component Destroy function
		*************************************************************************/
		void Destroy() override
		{
			DebuggerLog("Destroying Transform");
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the position of the game object transform

		\return
		 2D vector of the position of the game object(Transform)
		*************************************************************************/
		Vec2<float> GetPosition() const
		{
			return m_pos;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the position of the game object transform
		*************************************************************************/
		void SetPosition(Vec2<float> const& newPos)
		{
			m_pos = newPos;
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the scale of the game object transform

		\return
		 2D vector of the x and y scale of the game object(Transform)
		*************************************************************************/
		Vec2<float> GetScale() const
		{
			return m_scale;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the x and y scale of the game object transform
		*************************************************************************/
		void SetScale(Vec2<float> const& newScale)
		{
			m_scale = newScale;
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the rotation of the game object transform

		\return
		 Clockwise angle in radians of the game object
		*************************************************************************/
		float GetRotation() const
		{
			return m_angle;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the clockwise angle in radians of the game object
		 transform
		*************************************************************************/
		void SetRotation(float newRotation)
		{
			m_angle = newRotation;
		}

		/*!***********************************************************************
		\brief
		 Returns the Transform of the parent of this GameObject
		*************************************************************************/
		CPTransform* GetParent()
		{
			return m_parent;
		}

		/*!***********************************************************************
		\brief
		 Sets a new parent for thsi GameObject, if this GameObject already has a 
		 parent, it removes itself from the parent
		*************************************************************************/
		void SetParent(CPTransform* newParent)
		{
			// If parent exists, remove this from the old parent
			if (m_parent)
				m_parent->RemoveChild(this);

			m_parent = newParent;
			m_parent->AddChild(this);
		}

		/*!***********************************************************************
		\brief
		 Returns the child GameObject at the given index (A GameObject can have
		 many children) if the index is out of range an error will be thrown.
		*************************************************************************/
		CPTransform* GetChild(int index = 0)
		{
			if (index < 0 || index >= m_children.size())
				DebuggerLogErrorFormat("GameObject %s tried to get child at invalid index %d!", gameObj->GetName().c_str(), index);
			return m_children[index];
		}

		/*!***********************************************************************
		\brief
		 Returns the number of children this GameObject has
		*************************************************************************/
		int GetChildCount()
		{
			return (int)m_children.size();
		}

		/*!***********************************************************************
		\brief
		 Adds the GameObject to this GameObject's list of children
		*************************************************************************/
		void AddChild(CPTransform* newChild)
		{
			// Check if this object is already a child
			if (std::find(m_children.begin(), m_children.end(), newChild) != m_children.end()) return;

			m_children.push_back(newChild);
		}

		/*!***********************************************************************
		\brief
		 Removes the GameObject to this GameObject's list of children at the
		 given index
		*************************************************************************/
		void RemoveChild(int indexToRemove)
		{
			m_children.erase(m_children.begin() + indexToRemove);
		}

		/*!***********************************************************************
		\brief
		 Removes the GameObject to this GameObject's list of children by finding
		 if the pointer is the same as the one given
		*************************************************************************/
		void RemoveChild(CPTransform* childToRemove)
		{
			for (int index {0}; index < m_children.size(); ++index)
			{
				if (m_children[index] == childToRemove)
				{
					RemoveChild(index);
					return;
				}
			}
		}

		/*!***********************************************************************
		\brief
		 Repositions an object in the children list, commonly used in the editor
		 hierarchy.
		*************************************************************************/
		void ReorderChild(int oldIndex, int newIndex)
		{
			// Swap epending on whether the old index is before or after the new index using 2 while loops
			while (oldIndex > newIndex)
			{
				std::swap(m_children[oldIndex], m_children[oldIndex - 1]);
				--oldIndex;
			}
			while (oldIndex < newIndex - 1)
			{
				std::swap(m_children[oldIndex], m_children[oldIndex + 1]);
				++oldIndex;
			}
		}

		/*!***********************************************************************
		\brief
		 Removes all GameObjects from the list of children
		*************************************************************************/
		void RemoveAllChildren()
		{
			m_children.clear();
		}

	private:
		CPTransform* m_parent{ nullptr };		// Pointer to parent GameObject
		std::vector<CPTransform*> m_children{};	// Pointers to all children GameObjects 

		//---------------Global (World) Space---------------
		Vec2<float> m_pos{}, m_scale{ 1.0f, 1.0f };
		float m_angle{};

		// TODO: Children gameobjects in the future will have their position relative to their parent
		//---------------Local (World) Space---------------
		Vec2<float> m_localPos{}, m_localScale{ 1.0f, 1.0f };
		float m_localAngle{};
	};
}