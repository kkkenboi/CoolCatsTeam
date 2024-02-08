#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Components/CPPScriptComponent.h"
#include <sstream>
#include <iomanip>

namespace LB
{
	class CPPSFPS : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

	private:
		
		// Member variables to help with forming the FPS string
		GameObject* m_FPSObject{ nullptr };
		std::string fpsBuffer{};
		char finalFps[256];
		
		// Member variables to check for game settings
		bool m_GamePaused{ false };
		bool m_ShowFPS{ false };
	};
	REGISTER_SCRIPT(CPPSFPS)
}