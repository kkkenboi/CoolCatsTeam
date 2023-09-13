#pragma once
#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "stream.h"
#include "prettywriter.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "TestObjects.h"

using namespace rapidjson;

namespace Litterbox 
{	
	class JSONSerializer 
	{
	public:
		virtual bool Serialize(Value& data, Document::AllocatorType& allocator) = 0;
		virtual bool Deserialize(const Value& data)=0;
	};

	
	void SaveObject() 
	{
		TestGameObjectComponent joe;
		Document jsonFile;
		Document::AllocatorType& allocator = jsonFile.GetAllocator();
		if(joe.Serialize(jsonFile, allocator))
		{
			StringBuffer buffer;
			PrettyWriter<StringBuffer> jsonWriter(buffer);
			jsonFile.Accept(jsonWriter);
			//The final result should be in this string that we output to a file...
			std::string outputString = buffer.GetString();
		}
	}
	//probably put some filepath or something here
	void LoadObject()
	{
		/*if(jsonFile.Parse())*/
	}


}