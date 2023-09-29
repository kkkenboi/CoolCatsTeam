/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

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

	*************************************************************************/
	struct WindowsData
	{
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::string Title{};

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		unsigned int Width{}, Height{};

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double PosX{}, PosY{};

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		GLFWwindow* PtrToWindow{ nullptr };

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		WindowsData(const std::string& title = "Litterjox Engine",
			unsigned int width = 900,
			unsigned int height = 900)
			: Title(title), Width(width), Height(height), PtrToWindow{ nullptr }
		{
		}

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& allocator) 
		{
			data.SetObject();
			data.AddMember("Title", rapidjson::Value(Title.c_str(), allocator),allocator);
			data.AddMember("Width", Width,allocator);
			data.AddMember("Height", Height,allocator);
			data.AddMember("PosX", 0.0,allocator);
			data.AddMember("PosY", 0.0,allocator);
			return true;
		}

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		bool Deserialize(const Value& data)
		{
			bool HasTitle = data.HasMember("Title");
			bool HasWidth = data.HasMember("Width");
			bool HasHeight = data.HasMember("Height");
			if (data.IsObject())
			{
				if (HasTitle)
				{
					Title = data["Title"].GetString();
				}
				if (HasWidth && HasHeight)
				{
					Width = data["Width"].GetInt();
					Height = data["Height"].GetInt();
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

		*************************************************************************/
		WindowsSystem();

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		~WindowsSystem();

		/*!***********************************************************************
		 \brief
		 Update the system every frame
		*************************************************************************/
		virtual void Update();	

		/*!***********************************************************************
		 \brief
		 Update the system every frame
		*************************************************************************/
		virtual void Draw(WindowsData m_Data);				

		/*!***********************************************************************
		 \brief
		 Get the string name of the system, not the same as the window title
		*************************************************************************/
		void Initialize() override { SetSystemName("Windows System"); }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		virtual void SendMessage(Message* message) { UNREFERENCED_PARAMETER(message); };

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		unsigned int GetWidth()  const { return m_Data.Width;  }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		unsigned int GetHeight() const { return m_Data.Height; }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::string  GetTitle()  const { return m_Data.Title; }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double		 GetPosX()   const { return m_Data.PosX; }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		double		 GetPosY()   const { return m_Data.PosY;  }

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		GLFWwindow*  GetWindow() const { return m_Data.PtrToWindow; }

		// Callbacks
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		static void ErrorCB(int error, char const* description);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		static void FrameBufferCB(GLFWwindow* ptr_win, int width, int height);

		//static void KeyCB(GLFWwindow* pwin, int key, int scancode, int action, int mod);
		//static void MouseButtonCB(GLFWwindow* pwin, int button, int action, int mod);
		//static void MousePositionCB(GLFWwindow* pwin, double xpos, double ypos);
		//static void MouseScrollCB(GLFWwindow* pwin, double xoffset, double yoffset);

		//HWND hWnd;										//The handle to the game window
		//HINSTANCE hInstance;								//The handle to the instance
		//POINTS MousePosition;

	private:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		WindowsData m_Data;
	};

	/*!***********************************************************************
	 \brief

	*************************************************************************/
	class MessageQuit : public Message
	{
	public:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		MessageQuit() : Message(Mid::Quit) {};
	};

	/*!***********************************************************************
	 \brief

	*************************************************************************/
	extern WindowsSystem* WINDOWSSYSTEM;
}