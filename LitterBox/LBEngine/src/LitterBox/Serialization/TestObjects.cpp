#include "TestObjects.h"
#include <iostream>


//TEST HEADER FILES!!!
//NOT USED IN THE ENGINE!!!
//TURN AWAY!!!



namespace Litterbox
{
	Transform::Transform()
	{
		Position.x = 1.2f;
		Position.y = 3.4f;
		Rotation = 6.9f;
	}
}
//How it might be called somewhere else
// If you pull the files out, and put the code below in main, it should work.
	
	//TestGameObjectComponent joe;
	////   joe.ObjectName = "Joe";
	////   joe.ID = 123;
	//Transform test;

	//JSONSerializer stream;
	//Document jsonFile = stream.GetJSONFile("GameObjectData.json");
	//Document::AllocatorType& allocator = jsonFile.GetAllocator();



	//joe.Deserialize(jsonFile);
	//std::cout << joe.ObjectName << '\n';

	//if (joe.Serialize(jsonFile, allocator))
	//{
	//	stream.SaveToJSON("TestSaveData.json", jsonFile);
	//}
