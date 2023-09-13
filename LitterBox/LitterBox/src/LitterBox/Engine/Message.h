#pragma once

namespace LB
{
	namespace Mid // Message id
	{
		enum MessageIdType
		{
			None,
			Quit,
			Collide,
			ToggleDebugInfo,
			CharacterKey,
			MouseButton,
			MouseMove,
			FileDrop
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