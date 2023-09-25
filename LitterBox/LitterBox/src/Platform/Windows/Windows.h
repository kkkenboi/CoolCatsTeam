#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LitterBox/Core/System.h"
#include "LitterBox/Engine/Message.h"

#define UNREFERENCED_PARAMETER

namespace LB
{
	struct WindowsData
	{
		std::string Title{};
		unsigned int Width{}, Height{};
		double PosX{}, PosY{};
		GLFWwindow* PtrToWindow{ nullptr };

		WindowsData(const std::string& title = "LitterBox Engine",
			unsigned int width = 1600,
			unsigned int height = 900)
			: Title(title), Width(width), Height(height), PtrToWindow{ nullptr }
		{
		}
	};

	///Basic manager for windows. Implements the windows message pump and
	///broadcasts user input messages to all the systems.
	class WindowsSystem : public ISystem
	{
	public:
		WindowsSystem();
		~WindowsSystem();

		virtual void Update();								//Update the system every frame
		virtual void Draw(WindowsData m_Data);				//Update the system every frame
		void Initialize() override { SetSystemName("Windows System"); } //Get the string name of the system, not the same as the window title
		virtual void SendMessage(Message* message) { UNREFERENCED_PARAMETER(message); };


		unsigned int GetWidth()  const { return m_Data.Width;  }
		unsigned int GetHeight() const { return m_Data.Height; }
		std::string  GetTitle()  const { return m_Data.Title; }
		double		 GetPosX()   const { return m_Data.PosX; }
		double		 GetPosY()   const { return m_Data.PosY;  }
		GLFWwindow*  GetWindow() const { return m_Data.PtrToWindow; }

		// Callbacks
		static void ErrorCB(int error, char const* description);
		static void FrameBufferCB(GLFWwindow* ptr_win, int width, int height);
		static void KeyCB(GLFWwindow* pwin, int key, int scancode, int action, int mod);
		static void MouseButtonCB(GLFWwindow* pwin, int button, int action, int mod);
		static void MousePositionCB(GLFWwindow* pwin, double xpos, double ypos);
		static void MouseScrollCB(GLFWwindow* pwin, double xoffset, double yoffset);

		//HWND hWnd;										//The handle to the game window
		//HINSTANCE hInstance;								//The handle to the instance
		//POINTS MousePosition;

	private:
		WindowsData m_Data;
	};

	class MessageQuit : public Message
	{
	public:
		MessageQuit() : Message(Mid::Quit) {};
	};


	extern WindowsSystem* WINDOWSSYSTEM;
}