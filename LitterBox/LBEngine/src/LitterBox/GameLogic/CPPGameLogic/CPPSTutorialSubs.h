/*!************************************************************************
 * \file				CPPSTutorialSubs.h
 * \author(s)			Ryan Tan Jian Hao
 * \par DP email(s):  	ryanjianhao.tan@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/03/2024
 * \brief 				This source file contains the class definition
 * 						for the subtitles in the tutorial page
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Serialization/AssetManager.h"
#include <string>

namespace LB
{
	class CPPSTutorialSubs : public CPPBehaviour
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
		GameObject* mSubtitles{ nullptr }, *mCurtain{ nullptr };
		GameObject* mPlayer{ nullptr };
		std::string title{};
		float timer{ 0.f }, startingDelay{ 0.f };
		int currletter{ 0 };
		const char* titles[2];
	};

	REGISTER_SCRIPT(CPPSTutorialSubs)
}