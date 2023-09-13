#pragma once
#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "stream.h"
#include "prettywriter.h"
#include "filereadstream.h"
#include "filewritestream.h"


using namespace rapidjson;

namespace Litterbox 
{	
	class JSONSerializer 
	{
	public:
		virtual bool Serialize(Value& data, Document::AllocatorType& allocator) = 0;
		virtual bool Deserialize(const Value& data)=0;
	};

	
	

}