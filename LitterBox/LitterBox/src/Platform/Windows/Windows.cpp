#include "Windows.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"

#define UNREFERENCED_PARAMETER

namespace LB
{
    WindowsSystem* WINDOWSSYSTEM = nullptr;

	WindowsSystem::WindowsSystem()
	{
        // Ensure that there is only one window as it should only be called once for one engine
        // 
        // 
        // Set the global pointer to the windows system
        if (!WINDOWSSYSTEM)
        {
            WINDOWSSYSTEM = this;
        }
        else
        {
            std::cerr << "Windows System already exist\n";
        }

        if (!glfwInit()) {
            std::cout << "GLFW init has failed - abort program!!!" << std::endl;
        }

        // Set GLFW error callback
        glfwSetErrorCallback(ErrorCB);

        // Specify GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        //Deserialise the config settings
        LB::JSONSerializer stream;
        stream.DeserializeFromFile("config settings", m_Data);
        // Update Window Title


        // Create GLFW window
        m_Data.PtrToWindow = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
        if (!m_Data.PtrToWindow) {
            std::cerr << "GLFW unable to create OpenGL context - abort program\n";
            glfwTerminate();
            //return false;
        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(m_Data.PtrToWindow);

        // VSYNC OFF: 0, VSYNC ON: 1
        glfwSwapInterval(0);

        // Set GLFW callbacks
        glfwSetFramebufferSizeCallback(m_Data.PtrToWindow, FrameBufferCB);
        glfwSetKeyCallback(m_Data.PtrToWindow,  InvokeKeyPressed);
        glfwSetMouseButtonCallback(m_Data.PtrToWindow, InvokeKeyPressed);
        //glfwSetCursorPosCallback(m_Data.PtrToWindow, MousePositionCB);
        //glfwSetScrollCallback(m_Data.PtrToWindow, MouseScrollCB);

        glfwSetInputMode(m_Data.PtrToWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Initialize Glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize Glad" << std::endl;
            //return false;
        }

        int majorVersion, minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

        if (majorVersion >= 4 && minorVersion >= 5) {
            std::cout << "Using OpenGL version: " << majorVersion << "." << minorVersion << std::endl;
            std::cout << "Driver supports at least OpenGL 4.5\n" << std::endl;
        }
        else {
            std::cerr << "Driver doesn't support at least OpenGL 4.5 - abort program" << std::endl;
            //return false;
        }

        // Get the framebuffer size and set the viewport
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_Data.PtrToWindow, &fb_width, &fb_height);
        FrameBufferCB(m_Data.PtrToWindow, fb_width, fb_height);
	}

    WindowsSystem::~WindowsSystem()
	{
        glfwDestroyWindow(this->m_Data.PtrToWindow);
        glfwTerminate();
        // If there are any resources allocated, delete before destructing WindowsSystem
	}

	void WindowsSystem::Update()
	{
        if (glfwWindowShouldClose(this->m_Data.PtrToWindow)) 
        {
            MessageQuit q;
            CORE->BroadcastMessage(&q);
        }

        Draw(this->m_Data);

    }//Update the system every frame

    void WindowsSystem::Draw(WindowsData _m_Data)
    {
        std::string title{_m_Data.Title + " | FPS: " + std::to_string(1.0 / TIME->GetUnscaledDeltaTime())};

        // Set Window Title (Name + FPS)
        glfwSetWindowTitle(_m_Data.PtrToWindow, title.c_str());

        // Rendering portion


        // Swap buffer
        glfwSwapBuffers(_m_Data.PtrToWindow);

    }//Update the system every frame


    void WindowsSystem::ErrorCB(int error, char const* description)
    {
        UNREFERENCED_PARAMETER(error);
        UNREFERENCED_PARAMETER(description);
        #ifdef _DEBUG
            std::cerr << "GLFW error: " << description << std::endl;
        #endif

    }
    void WindowsSystem::FrameBufferCB(GLFWwindow* ptr_win, int width, int height)
    {
        UNREFERENCED_PARAMETER(ptr_win);
        UNREFERENCED_PARAMETER(width);
        UNREFERENCED_PARAMETER(height);
        #ifdef _DEBUG
            std::cout << "FrameBufferCB getting called!" << std::endl;
        #endif
    }
}
