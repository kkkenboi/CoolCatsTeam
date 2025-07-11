/*!************************************************************************
 \file				Windows.cpp
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

#include "Windows.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Scene/SceneManager.h"

#include "stb_image.h"

#define UNREFERENCED_PARAMETER

namespace LB
{
    /*!***********************************************************************
     \brief
     Callback function that is in charge of minimizing window on unfocus.
    *************************************************************************/
    void exit() {
        glfwIconifyWindow(WINDOWSSYSTEM->GetWindow());
    }

    WindowsSystem* WINDOWSSYSTEM = nullptr;
    bool previousState = false;

    // TODO: refactor pausing on window close
    bool wasPausedBeforeUnfocus = false;

    /*!***********************************************************************
     \brief
     Initialises the WindowsSystem with GLFW functions, specifying
    *************************************************************************/
    WindowsSystem::WindowsSystem()
	{
        // Ensure that there is only one window as it should only be 
        // called once for the engine
        if (!WINDOWSSYSTEM)
        {
            WINDOWSSYSTEM = this;
        }
        else
        {
            DebuggerLogError("Windows System already exist");
        }

        if (!glfwInit()) {
            DebuggerLogError("GLFW init has failed - abort program!!!");
        }

        // Set GLFW error callback
        glfwSetErrorCallback(ErrorCB);

        // Specify GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Setting Monitor and Video Mode
        m_Data.m_Monitor = glfwGetPrimaryMonitor();
        m_Data.m_VideoMode = glfwGetVideoMode(m_Data.m_Monitor);
        
        // Set Monitor width and height
        glfwGetMonitorWorkarea(m_Data.m_Monitor, nullptr, nullptr, &monitor_width, &monitor_height);

        //Deserialise the config settings
        //Current file path is : \LBEditor\Library\Jason\config settings.json"
        LB::JSONSerializer::DeserializeFromFile("config settings", m_Data);

        if (CORE->IsEditorMode())
        {
            // Create GLFW window
            m_Data.m_Width = 1920;
            m_Data.m_Height = 1080;
            m_Data.m_PtrToWindow = glfwCreateWindow(m_Data.m_Width, m_Data.m_Height, m_Data.m_Title.c_str(), NULL, NULL);
            if (!m_Data.m_PtrToWindow) {
                DebuggerLogError("GLFW unable to create OpenGL context - abort program");
                glfwTerminate();
            }

            used_width = m_Data.m_Width;
            used_height = m_Data.m_Height;
        }
        else // Game Mode
        {
            glfwWindowHint(GLFW_RED_BITS, m_Data.m_VideoMode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, m_Data.m_VideoMode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, m_Data.m_VideoMode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, m_Data.m_VideoMode->refreshRate);

            // Update Window Title

            // Create GLFW window

            m_Data.m_PtrToWindow = glfwCreateWindow(m_Data.m_VideoMode->width, m_Data.m_VideoMode->height, m_Data.m_Title.c_str(), m_Data.m_Monitor, NULL); // Fullscreen mode
            if (!m_Data.m_PtrToWindow) {
                DebuggerLogError("GLFW unable to create OpenGL context - abort program");
                glfwTerminate();
            }
            used_width = m_Data.m_VideoMode->width;
            used_height = m_Data.m_VideoMode->height;

        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(m_Data.m_PtrToWindow);

        // VSYNC OFF: 0, VSYNC ON: 1
        glfwSwapInterval(0);

        // Set GLFW callbacks
        glfwSetFramebufferSizeCallback(m_Data.m_PtrToWindow, FrameBufferCB);
        glfwSetKeyCallback(m_Data.m_PtrToWindow, GLFWKeyPressed);
        glfwSetMouseButtonCallback(m_Data.m_PtrToWindow, GLFWKeyPressed);
        glfwSetInputMode(m_Data.m_PtrToWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(m_Data.m_PtrToWindow, GLFWMousePos);
        glfwSetWindowFocusCallback(m_Data.m_PtrToWindow, FocusCB);

        // Initialize Glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            DebuggerLogError("Failed to initialize Glad");
            //return false;
        }

        int majorVersion, minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

        if (majorVersion >= 4 && minorVersion >= 5) {
            DebuggerLogFormat("Using OpenGL version: %d.%d", majorVersion, minorVersion);
            DebuggerLog("Driver supports at least OpenGL 4.5");

        }
        else {
            DebuggerLogError("Driver doesn't support at least OpenGL 4.5 - abort program");
        }

        // Get the framebuffer size and set the viewport
        int fb_width, fb_height;
        glfwGetFramebufferSize(m_Data.m_PtrToWindow, &fb_width, &fb_height);
        FrameBufferCB(m_Data.m_PtrToWindow, fb_width, fb_height);

        if(!CORE->IsEditorMode())
            OnApplicationUnFocus.Subscribe(exit);

        SetSystemName("Windows System");
    }

    /*!***********************************************************************
     \brief
     Destroys the WindowsSystems with GLFW functions
    *************************************************************************/
    WindowsSystem::~WindowsSystem()
	{
        // If there are any resources allocated, delete before destructing WindowsSystem
        glfwDestroyCursor(m_Data.m_Cursor);

        // Finally destroy the window context and terminate glfw
        glfwDestroyWindow(this->m_Data.m_PtrToWindow);
        glfwTerminate();
	}

    /*!***********************************************************************
     \brief
     Initializes other GLFW functions, such as the mouse, that isn't possible
     through the constructor.
    *************************************************************************/
    void WindowsSystem::Initialize()
    {
        // Set the window cursor       
        GLFWimage image{};
        image.width = ASSETMANAGER->Textures[ASSETMANAGER->assetMap["Finger"]].first->width;
        image.height = ASSETMANAGER->Textures[ASSETMANAGER->assetMap["Finger"]].first->height;

        // - Grabbing the actual values from the texture buffer itself
        GLuint* pixels = new GLuint[image.width * image.height * STBI_rgb_alpha];
        glBindTexture(GL_TEXTURE_2D, ASSETMANAGER->GetTextureIndex("Finger"));
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        image.pixels = reinterpret_cast<unsigned char*>(pixels);

        // - Create an image for the cursor
        m_Data.m_Cursor = glfwCreateCursor(&image, 0, 0);

        // Free memory
        delete[] pixels;

        // Subscribe for mouse change
        SCENEMANAGER->onNewSceneLoadString.Subscribe(LB::UpdateCursor);
        TIME->onPauseEvent.Subscribe(LB::UpdateCursor);
    }

    /*!***********************************************************************
     \brief
     Checks if the window is closing each frame and sets the window title 
     every frame

     \return
     Nothing
    *************************************************************************/
    void WindowsSystem::Update()
    {
        if (glfwWindowShouldClose(this->m_Data.m_PtrToWindow))
        {
            MessageQuit q;
            CORE->BroadcastMessage(&q);
        }
        //Keeps track if the window is focused or not. On focus, it only triggers ONCE. 
        bool isFocused = glfwGetWindowAttrib(this->m_Data.m_PtrToWindow, GLFW_FOCUSED);
        if (isFocused && !previousState)
        {
            previousState = true;
            OnApplicationFocus.Invoke();
        }
        if (!isFocused)
        {
            previousState = false;
        }

        if (!CORE->IsEditorMode())
        {
            std::string title{this->m_Data.m_GameTitle};

            // Set Window Title (Name + FPS)
            glfwSetWindowTitle(this->m_Data.m_PtrToWindow, title.c_str());
            glfwSetCursor(m_Data.m_PtrToWindow, m_Data.m_Cursor);

            if (INPUT->IsKeyTriggered(KeyCode::KEY_L))
            {
                toggleFullScreen();
            }
        }
        else
        {
            // If in editor mode, display the FPS in the title bar
            std::string title{ this->m_Data.m_Title + " | FPS: " + std::to_string(1.0 / TIME->GetUnscaledDeltaTime()) };
            //std::string title{ this->m_Data.m_Title + " | FPS: " + std::to_string(TIME->GetFrameCount() / TIME->GetTime()) };

            // Set Window Title (Name + FPS)
            glfwSetWindowTitle(this->m_Data.m_PtrToWindow, title.c_str());
        }
        Draw(this->m_Data);   
    }

    /*!***********************************************************************
     \brief
     Swaps framebuffer 

     \return
     Nothing
    *************************************************************************/
    void WindowsSystem::Draw(WindowsData _m_Data)
    {
        glfwSwapBuffers(_m_Data.m_PtrToWindow);
    }

    /*!***********************************************************************
     \brief
     Sends a message to other systems

     \return
     Nothing
    *************************************************************************/
    void WindowsSystem::SendMessage(Message* message) 
    { 
        UNREFERENCED_PARAMETER(message); 
    };

    /*!***********************************************************************
     \brief
     Gets the width of the current window

     \return
     Width of current window
    *************************************************************************/
    unsigned int WindowsSystem::GetWidth() const 
    { 
        return used_width; 
    }

    /*!***********************************************************************
     \brief
     Gets the height of the current window

     \return
     Height of current window
    *************************************************************************/
    unsigned int WindowsSystem::GetMonitorHeight() const
    {

        return monitor_height;
    }

    /*!***********************************************************************
     \brief
     Gets the width of the current window

     \return
     Width of current window
    *************************************************************************/
    unsigned int WindowsSystem::GetMonitorWidth() const
    {
        return monitor_width;
    }

    /*!***********************************************************************
     \brief
     Gets the height of the current window

     \return
     Height of current window
    *************************************************************************/
    unsigned int WindowsSystem::GetHeight() const
    {
        return used_height;
    }

    /*!***********************************************************************
     \brief
     Gets the title of the current window

     \return
     Title of current window
    *************************************************************************/
    std::string WindowsSystem::GetTitle() const
    { 
        return m_Data.m_Title; 
    }

    /*!***********************************************************************
     \brief
     Gets the x position of the current window

     \return
     Width of current window
    *************************************************************************/
    double WindowsSystem::GetPosX() const
    { 
        return m_Data.m_PosX; 
    }

    /*!***********************************************************************
     \brief
     Gets the y position the current window

     \return
     Width of current window
    *************************************************************************/
    double WindowsSystem::GetPosY() const
    { 
        return m_Data.m_PosY; 
    }

    /*!***********************************************************************
     \brief
     Gets the pointer to the current window

     \return
     Pointer to current window
    *************************************************************************/
    GLFWwindow* WindowsSystem::GetWindow() const
    { 
        return m_Data.m_PtrToWindow; 
    }

    /*!***********************************************************************
     \brief
     Sets the Error callback for GLFW

     \return
     Nothing
    *************************************************************************/
    void WindowsSystem::ErrorCB(int error, char const* description)
    {
        UNREFERENCED_PARAMETER(error);
        UNREFERENCED_PARAMETER(description);
        #ifdef _DEBUG
            std::cerr << "GLFW error: " << description << std::endl;
        #endif

    }

    /*!***********************************************************************
     \brief
     Sets the FrameBuffer callback for GLFW

     \return
     Nothing
    *************************************************************************/
    void WindowsSystem::FrameBufferCB(GLFWwindow* ptr_win, int width, int height)
    {
        UNREFERENCED_PARAMETER(ptr_win);
        UNREFERENCED_PARAMETER(width);
        UNREFERENCED_PARAMETER(height);

        // This is to ensure that the window is still drawing when 
        // resizing the window
        WINDOWSSYSTEM->Draw(WINDOWSSYSTEM->m_Data);
    }

    /*!***********************************************************************
     \brief
     Sets the Window Focus callback
    *************************************************************************/
    void WindowsSystem::FocusCB(GLFWwindow* window, int focused)
    {
        UNREFERENCED_PARAMETER(window);
        if (focused)
        {
            if (!wasPausedBeforeUnfocus)
            {
                TIME->Pause(false);
            }
            WINDOWSSYSTEM->OnApplicationFocus.Invoke();
        }
        else
        {
            wasPausedBeforeUnfocus = TIME->IsPaused();

            if (!wasPausedBeforeUnfocus) TIME->Pause(true);
            WINDOWSSYSTEM->OnApplicationUnFocus.Invoke();
        }
    }

    /*!***********************************************************************
     \brief
     Toggles the screen view to fullscreen mode or windowed mode. This is 
     mainly used when the window is running in Game Mode.
    *************************************************************************/
    void WindowsSystem::toggleFullScreen()
    {
        m_Data.m_FullscreenMode = !m_Data.m_FullscreenMode;

        if (m_Data.m_FullscreenMode)
        {
            glfwSetWindowMonitor(m_Data.m_PtrToWindow, m_Data.m_Monitor, 0, 0, m_Data.m_VideoMode->width, m_Data.m_VideoMode->height, NULL);
            used_width = m_Data.m_VideoMode->width;
            used_height = m_Data.m_VideoMode->height;
        }
        else
        {
            glfwSetWindowMonitor(m_Data.m_PtrToWindow, NULL, m_Data.m_VideoMode->width / 4, m_Data.m_VideoMode->height / 4, m_Data.m_Width, m_Data.m_Height, NULL);

            used_width = m_Data.m_Width;
            used_height = m_Data.m_Height;
        }

        screenSizeChange.Invoke();
    }


    /*!***********************************************************************
     \brief
     Updates crucial values required to convert mouse position from screen space
     to camera space.

     \param
     xVP - The width of the viewport which the game is rendering on

     \param
     yVP - The height of the viewport which the game is rendering on

     \param
     hBO - The height of the border offset from the bottom of the screen to
     the bottome of the viewport.
    *************************************************************************/
    void WindowsSystem::updateScreenSize(float xVP, float yVP, float hBO)
    {
        xVPConversion = 1920.f / xVP;
        yVPConversion = 1080.f / yVP;
        heightBorderOffset = hBO;
    }

    /*!***********************************************************************
    \brief
    This function updates the cursor based on the scene's name
    *************************************************************************/
    void WindowsSystem::UpdateCursor(std::string sceneName)
    {
        if (sceneName == "SceneMainMenu" || sceneName == "SceneCutscene" || sceneName == "SceneSplashScreen" || sceneName == "Paused")
        {
            m_CurrentSceneName = "Finger";
            //DebuggerLogFormat("%s, This is the scene name :D", sceneName.c_str());
        }
        else // Means this is a level scene / playing
        {
            m_CurrentSceneName = "Target";
        }

        // Destroy the current cursor first
        if (m_Data.m_Cursor)
        {
            glfwDestroyCursor(m_Data.m_Cursor);
        }

        // Set the window cursor       
        GLFWimage image{};
        image.width = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[m_CurrentSceneName]].first->width;
        image.height = ASSETMANAGER->Textures[ASSETMANAGER->assetMap[m_CurrentSceneName]].first->height;

        // - Grabbing the actual values from the texture buffer itself
        GLuint* pixels = new GLuint[image.width * image.height * STBI_rgb_alpha];
        glBindTexture(GL_TEXTURE_2D, ASSETMANAGER->GetTextureIndex(m_CurrentSceneName));
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        image.pixels = reinterpret_cast<unsigned char*>(pixels);

        // - Create an image for the cursor
        if (sceneName == "Target")
        {
            m_Data.m_Cursor = glfwCreateCursor(&image, image.width / 2, image.height / 2);
        }
        else
        {
            m_Data.m_Cursor = glfwCreateCursor(&image, 0, 0);
        }

        // Free memory
        delete[] pixels;
    }

    std::string const& WindowsSystem::GetSceneName()
    {
        return m_CurrentSceneName;
    }

    /*!***********************************************************************
    \brief
    Event to update the cursor based on the scene name
    *************************************************************************/
    void UpdateCursor(std::string sceneName)
    {
        WINDOWSSYSTEM->UpdateCursor(sceneName);
    }
}
