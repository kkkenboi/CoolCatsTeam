#include "Platform/Windows/Windows.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/GameLogic/GameLogic.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Debugging/Profiler.h"
#include "LitterBox/Physics/RigidBodyManager.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Audio/AudioManager.h"
#include "Core.h"

namespace LB {
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		LBEngine* Engine;
	};

	// To be used in Client
	Application* CreateApplication();

}

extern LB::Application* LB::CreateApplication();
