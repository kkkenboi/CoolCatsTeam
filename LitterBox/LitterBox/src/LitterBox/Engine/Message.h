/*!************************************************************************
 \file				Message.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains functions for the Message class that
					allows systems to send messages to each other. 
					Currently allows the program to exit.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

namespace LB
{
	enum MessageIDType
	{
		None,
		Quit,
	};

	/*!***********************************************************************
	 \brief
	 Message class that creates a specific type of message to be sent. If the
	 receiver is set to respond to that type, it will act on it.
	*************************************************************************/
	class Message
	{
	public:

		/*!***********************************************************************
		 \brief
		 Message class that creates a specific type of message to be sent. If the
		 receiver is set to respond to that type, it will act on it.
		*************************************************************************/
		Message(MessageIDType ID) : MessageID(ID) {};
		/*!***********************************************************************
		 \brief
		 Message class that creates a specific type of message to be sent. If the
		 receiver is set to respond to that type, it will act on it.
		*************************************************************************/
		virtual ~Message() {};

		MessageIDType MessageID;
	};
}