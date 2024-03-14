/*!************************************************************************
 \file				TransformComponent.cpp
 \author(s)			Amadeus Chia Jinhan, Ang Jiawei Jarrett
 \par DP email(s):	amadeusjinhan.chia@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				02/11/2023
 \brief				This file contains the implementation of the transform
					Component Deserialize function (it's here to avoid
					some weird circular dependency issue)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "LitterBox/Components/TransformComponent.h"
#include "LitterBox/Factory/GameObjectFactory.h"

namespace LB
{
	/*!***********************************************************************
	 * \brief Deserializes json data into a Transform Component
	**************************************************************************/
	bool CPTransform::Deserialize(const Value& data)
	{	//Same thing, we check if it has all the members
		bool HasPosition = data.HasMember("Position");
		bool HasScale = data.HasMember("Scale");
		bool HasRot = data.HasMember("Rotation");
		bool HasChildren = data.HasMember("Children");
		//We just log it for now
		//DebuggerLog("Deserializing Transform");
		if (data.IsObject())
		{	//Every transform needs this, so we check 
			if (HasPosition && HasScale && HasRot)
			{
				const Value& positionValue = data["Position"];
				const Value& scaleValue = data["Scale"];
				const Value& rotationValue = data["Rotation"];
				m_pos.Deserialize(positionValue);
				m_scale.Deserialize(scaleValue);
				m_angle = rotationValue.GetFloat();
				//Not every transform will have a child
				if (HasChildren)
				{	//We just save it into a json array 
					const Value& childrenValue = data["Children"].GetArray();
					for (rapidjson::SizeType i{}; i < childrenValue.Size(); ++i)
					{
						GameObject* child = FACTORY->SpawnGameObject();
						child->Deserialize(childrenValue[i]);

						this->DeserializeAddChild(child->GetComponent<CPTransform>());
						child->GetComponent<CPTransform>()->SetParent(this);
					}
				}
				return true;
			}
		}
		return false;
	}
}