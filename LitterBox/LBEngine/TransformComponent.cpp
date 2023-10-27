#include "LitterBox/Components/TransformComponent.h"
#include "LitterBox/Factory/GameObjectFactory.h"

namespace LB
{
	bool CPTransform::Deserialize(const Value& data)
	{
		bool HasPosition = data.HasMember("Position");
		bool HasScale = data.HasMember("Scale");
		bool HasRot = data.HasMember("Rotation");
		bool HasChildren = data.HasMember("Children");
		DebuggerLog("Deserializing Transform");
		if (data.IsObject())
		{
			if (HasPosition && HasScale && HasRot)
			{
				const Value& positionValue = data["Position"];
				const Value& scaleValue = data["Scale"];
				const Value& rotationValue = data["Rotation"];
				m_pos.Deserialize(positionValue);
				m_scale.Deserialize(scaleValue);
				m_angle = rotationValue.GetFloat();
				if (HasChildren)
				{
					const Value& childrenValue = data["Children"].GetArray();
					for (rapidjson::SizeType i{}; i < childrenValue.Size(); ++i)
					{
						GameObject* child = FACTORY->SpawnGameObject();
						child->Deserialize(childrenValue[i]);
						this->AddChild(child->GetComponent<CPTransform>());
						child->GetComponent<CPTransform>()->SetParent(this);
					}
				}
				return true;
			}
		}
		return false;
	}
}