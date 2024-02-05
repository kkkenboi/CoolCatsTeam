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
		GameObject* m_FPSObject{ nullptr };
		std::string fpsBuffer{};
		char finalFps[256];
		bool m_GamePaused{ false };
	};
	REGISTER_SCRIPT(CPPSFPS)
}