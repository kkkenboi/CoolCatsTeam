#pragma once

namespace LB
{
	namespace Mid // Message id
	{
		enum MessageIdType
		{
			None,
			Quit,
		};
	}

	class Message
	{
	public:
		Message(Mid::MessageIdType id) : MessageId(id) {};
		Mid::MessageIdType MessageId;
		virtual ~Message() {};
	};
}