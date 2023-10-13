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
		ComponentTypeID GetType() override
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
			std::cout << "Serialising Transform\n";
			data.SetObject();
			Value PositionValue;
			if (pos.Serialize(PositionValue, alloc))
			{
				data.AddMember("Position", PositionValue, alloc);
			}
			else return false;
			Value ScaleValue;
			if (scale.Serialize(ScaleValue, alloc))
			{
				data.AddMember("Scale", ScaleValue, alloc);
			}
			else return false;
			data.AddMember("Rotation", angle, alloc);
			return true;
		}

		/*!***********************************************************************
		\brief
		 Function will save data from scene to json file.

		\return
		 returns true if deserialization succeeds and false if not.
		*************************************************************************/
		bool Deserialize(const Value& data) override
		{
			bool HasPosition = data.HasMember("Position");
			bool HasScale = data.HasMember("Scale");
			bool HasRot = data.HasMember("Rotation");
			std::cout << "Deserialising Transform\n";
			if (data.IsObject())
			{
				if (HasPosition && HasScale && HasRot)
				{
					const Value& positionValue = data["Position"];
					const Value& scaleValue = data["Scale"];
					const Value& rotationValue = data["Rotation"];
					pos.Deserialize(positionValue);
					scale.Deserialize(scaleValue);
					angle = rotationValue.GetFloat();
					return true;
				}
			}
			return false;
		}

		/*!***********************************************************************
		\brief
		 Component Destroy function
		*************************************************************************/
		void Destroy() override
		{
			std::cout << "Destroying Transform\n";
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the position of the game object transform

		\return
		 2D vector of the position of the game object(Transform)
		*************************************************************************/
		Vec2<float> GetPosition() const
		{
			return pos;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the position of the game object transform
		*************************************************************************/
		void SetPosition(Vec2<float> const& newPos)
		{
			pos = newPos;
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the scale of the game object transform

		\return
		 2D vector of the x and y scale of the game object(Transform)
		*************************************************************************/
		Vec2<float> GetScale() const
		{
			return scale;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the x and y scale of the game object transform
		*************************************************************************/
		void SetScale(Vec2<float> const& newScale)
		{
			scale = newScale;
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the rotation of the game object transform

		\return
		 Clockwise angle in radians of the game object
		*************************************************************************/
		float GetRotation() const
		{
			return angle;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the clockwise angle in radians of the game object
		 transform
		*************************************************************************/
		void SetRotation(float newRotation)
		{
			angle = newRotation;
		}

	private:
		Vec2<float> pos{}, scale{ 1.0f, 1.0f };
		float angle{};
	};
}