#pragma once
#include "Serializer.h"
namespace Litterbox 
{
	struct Vector2 :JSONSerializer {
		float x, y;		

		// Inherited via JSONSerializer
		virtual bool Serialize(Value& data, Document::AllocatorType& allocator)
		{
			data.SetObject();
			data.AddMember( "x",x,allocator);
			data.AddMember( "y",y,allocator);
			return true;//serialization pass
		}
		virtual bool Deserialize(const Value& data)
		{
			bool HasXValue = data.HasMember("x");
			bool HasYValue = data.HasMember("y");
			if (data.IsObject())
			{
				if (HasXValue && HasYValue)
				{
					x = data["x"].GetFloat();
					y = data["y"].GetFloat();
					return true; //hopefully it passes ;-;
				}
			}
			return false;
		}
	};
	class TestGameObjectComponent : JSONSerializer
	{
	public:
		std::string ObjectName;
		int ID;
		// Inherited via JSONSerializer
		virtual bool Serialize(Value& data, Document::AllocatorType& allocator)
		{			
			data.SetObject();
			data.AddMember("name", ObjectName, allocator);
			data.AddMember("ID", ID, allocator);
		}
		virtual bool Deserialize(const Value& data)
		{
			bool HasObjectName = data.HasMember("name");
			bool HasID = data.HasMember("ID");
			if (data.IsObject())
			{
				if (HasObjectName && HasID)
				{
					ObjectName = data["name"].GetString();
					ID = data["ID"].GetInt();
				}
			}
		}
	};
	class Transform : public TestGameObjectComponent
	{
	public:
		Transform();
		Vector2 Position;
		float Rotation;
		
		virtual bool Serialize(Value& data, Document::AllocatorType& allocator)
		{
			if (TestGameObjectComponent::Serialize(data, allocator))
			{
				Value positionValue;
				//We need to check if the position exists...
				if (Position.Serialize(positionValue, allocator))
				{
					data.AddMember("Position", positionValue, allocator);
				}
				else
				{	//throw some error because serialization of postion failed!
					return false;
				}
				data.AddMember("Rotation", Rotation, allocator);
			}
			return true;
		}

		virtual bool Deserialize(Value& data)
		{
			bool HasPosition = data.HasMember("Position");
			bool HasRotation = data.HasMember("Rotation");
			if (data.IsObject())
			{
				if (TestGameObjectComponent::Deserialize(data))
				{
					if (HasPosition && HasRotation)
					{
						Value& positionValue = data["Position"];
						if (Position.Deserialize(positionValue))
						{
							Rotation = data["Rotation"].GetFloat();
						}
					}
				}
			}
		}
	};
}