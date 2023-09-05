#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LitterBox/Core/System.h"

namespace LitterBox
{
	struct WindowsData
	{
		std::string Title{};
		unsigned int Width{}, Height{};
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

		virtual void Update(float dt);						//Update the system every frame
		virtual void Draw(WindowsData m_Data);				//Update the system every frame
		virtual std::string GetName() { return "Windows"; }	//Get the string name of the system, not the same as the window title

		unsigned int GetWidth()  const { return m_Data.Width;  }
		unsigned int GetHeight() const { return m_Data.Height; }
		std::string  GetTitle()  const { return m_Data.Title;  }

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

	//extern WindowsSystem* WINDOWSSYSTEM;
}