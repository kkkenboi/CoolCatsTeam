#pragma once

#include "Platform/Windows/Windows.h"
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Engine/Input.h"
namespace LB
{
	class CPPSAimUI : public CPPBehaviour
	{
	public: 
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPTransform* mTransform;
		GameObject* mPlayer;
		Vec2<float> MousePos;
		Vec2<float> DirToMouse;
	};
	REGISTER_SCRIPT(CPPSAimUI)

}

