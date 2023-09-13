#include "pch.h"
#include "TestObjects.h"

namespace Litterbox 
{
	Transform::Transform()
	{
		Position.x = 1.2f;
		Position.y = 3.4f;
		Rotation = 6.9f;
	}
	//void SaveObject()
	//{
	//	TestGameObjectComponent joe;
	//	Document jsonFile;
	//	Document::AllocatorType& allocator = jsonFile.GetAllocator();
	//	if (joe.Serialize(jsonFile, allocator))
	//	{
	//		StringBuffer buffer;
	//		PrettyWriter<StringBuffer> jsonWriter(buffer);
	//		jsonFile.Accept(jsonWriter);
	//		//The final result should be in this string that we output to a file...
	//		std::string outputString = buffer.GetString();
	//	}
	//}
	////probably put some filepath or something here
	//void LoadObject()
	//{
	//	/*if(jsonFile.Parse())*/
	//}

}