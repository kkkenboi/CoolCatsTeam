/*!************************************************************************
 \file				Windows.h
 \author(s)			Kenji Brannon Chong | Amadeus Chia Jinhan
 \par DP email(s):	kenjibrannon.c@digipen.edu | amadeusjinhan.chia@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions definitions of the Windows class. It holds all
 of the current window's data and provides callbacks for GLFW, such as Error
 and FrameBuffer.

 This file also contains functions definitions of the MessageQuit class to
 allow the Windows system to receive a message to quit.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Message.h"
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Holds all of the data within the Window, eg. width and height and pointer
	 to the window.
	*************************************************************************/
	struct WindowsData
	{
		std::string		m_Title{};
		std::string		m_GameTitle{};
		unsigned int	m_Width{}, m_Height{};
		double			m_PosX{}, m_PosY{};
		bool			m_FullscreenMode{ true };

		GLFWwindow* m_PtrToWindow{ nullptr };
		const GLFWvidmode* m_VideoMode{ nullptr };
		GLFWmonitor* m_Monitor{ nullptr };

		/*!***********************************************************************
		 \brief
		 Constructor for WindowsData
		*************************************************************************/
		WindowsData(const std::string& title = "LitterBox Engine", const std::string& gameTitle = "Purrfect Putt",
			unsigned int width = 900,
			unsigned int height = 900)
			: m_Title(title), m_GameTitle(gameTitle), m_Width(width), m_Height(height), m_PtrToWindow{ nullptr }
		{
		}

		/*!***********************************************************************
		 \brief
		 Serializes the configuration of the window's data to a file

		 \return
		 True if serialized
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& allocator) 
		{
			data.SetObject();
			data.AddMember("Title", rapidjson::Value(m_Title.c_str(), allocator),allocator);
			data.AddMember("Width", m_Width,allocator);
			data.AddMember("Height", m_Height,allocator);
			data.AddMember("PosX", 0.0,allocator);
			data.AddMember("PosY", 0.0,allocator);
			data.AddMember("FullScreenMode", m_FullscreenMode, allocator);
			return true;
		}

		/*!***********************************************************************
		 \brief
		 Deserializes the configuration of the window's data from a file

		 \return
		 True if serialized
		*************************************************************************/
		bool Deserialize(const Value& data)
		{
			bool HasTitle = data.HasMember("Title");
			bool HasWidth = data.HasMember("Width");
			bool HasHeight = data.HasMember("Height");
			bool HasFullScreen = data.HasMember("FullScreenMode");

			if (data.IsObject())
			{
				if (HasTitle)
				{
					m_Title = data["Title"].GetString();
				}
				if (HasWidth && HasHeight)
				{
					m_Width = data["Width"].GetInt();
					m_Height = data["Height"].GetInt();
				}
				if (HasFullScreen)
				{
					m_FullscreenMode = data["FullScreenMode"].GetBool();
				}
				return true;
			}
			return false;
		}
	};

	/*!***********************************************************************
	\brief
	 Basic manager for windows. Implements the windows message pump and
	 broadcasts user input messages to all the systems.
	*************************************************************************/
	class WindowsSystem : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initialises the WindowsSystem with GLFW functions, specifying 
		*************************************************************************/
		WindowsSystem();

		/*!***********************************************************************
		 \brief
		 Destroys the WindowsSystems with GLFW functions
		*************************************************************************/
		~WindowsSystem();

		/*!***********************************************************************
		 \brief
		 Checks if the window is closing each frame and sets the window title
		 every frame
		*************************************************************************/
		void Update();	

		/*!***********************************************************************
		 \brief
		 Swaps framebuffer
		*************************************************************************/
		void Draw(WindowsData m_Data);				

		/*!***********************************************************************
		 \brief
		 Sends a message to other systems
		*************************************************************************/
		void SendMessage(Message* message);

		/*!***********************************************************************
		 \brief
		 Gets the width of the current window
		*************************************************************************/
		unsigned int GetWidth() const;

		/*!***********************************************************************
		 \brief
		 Gets the height of the current window
		*************************************************************************/
		unsigned int GetHeight() const;

		/*!***********************************************************************
		 \brief
		 Gets the height of the current window
		*************************************************************************/
		unsigned int GetMonitorHeight() const;

		/*!***********************************************************************
		 \brief
		 Gets the height of the current window
		*************************************************************************/
		unsigned int GetMonitorWidth() const;

		/*!***********************************************************************
		 \brief
		 Gets the title of the current window
		*************************************************************************/
		std::string GetTitle() const;

		/*!***********************************************************************
		 \brief
		 Gets the x position of the current window
		*************************************************************************/
		double GetPosX() const;

		/*!***********************************************************************
		 \brief
		 Gets the y position the current window
		*************************************************************************/
		double GetPosY() const;

		/*!***********************************************************************
		 \brief
		 Gets the pointer to the current window
		*************************************************************************/
		GLFWwindow* GetWindow() const;

		// Callbacks
		/*!***********************************************************************
		 \brief
		 Sets the Error callback for GLFW
		*************************************************************************/
		static void ErrorCB(int error, char const* description);

		/*!***********************************************************************
		 \brief
		 Sets the FrameBuffer callback for GLFW
		*************************************************************************/
		static void FrameBufferCB(GLFWwindow* ptr_win, int width, int height);

		/*!***********************************************************************
		 \brief
		 Sets the Window Focus callback
		*************************************************************************/
		static void FocusCB(GLFWwindow* window, int focused);

		/*!***********************************************************************
		 \brief
		 Gets the width of the main monitor in pixels
		*************************************************************************/
		inline unsigned int GetScreenWidth() { return m_Data.m_VideoMode->width; }

		/*!***********************************************************************
		 \brief
		 Gets the height of the main monitor in pixels
		*************************************************************************/
		inline unsigned int GetScreenHeight() { return m_Data.m_VideoMode->height; }
		
		/*!***********************************************************************
		 \brief
		 Checks if the current window context is in fullscreen mode or not
		*************************************************************************/
		inline bool			IsFullScreen() { return m_Data.m_FullscreenMode; }

		/*!***********************************************************************
		 \brief
		 Toggles the screen view to fullscreen mode or windowed mode. This is
		 mainly used when the window is running in Game Mode.
		*************************************************************************/
		void				toggleFullScreen();

		Event<> OnApplicationFocus;
		Event<> OnApplicationUnFocus;
		Event<> screenSizeChange;

	private:
		WindowsData m_Data;
		unsigned int used_width;
		unsigned int used_height;
		int monitor_width;
		int monitor_height;
	};

	/*!***********************************************************************
	 \brief
	 A specific type of Message to quit the LitterBox Engine
	*************************************************************************/
	class MessageQuit : public Message
	{
	public:
		/*!***********************************************************************
		 \brief
		 Constructor for MessageQuit
		*************************************************************************/
		MessageQuit() : Message(Quit) {};
	};

	extern WindowsSystem* WINDOWSSYSTEM;
}