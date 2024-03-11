/*!************************************************************************
 \file				ScriptRegistration.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains a stopgap solution for the struct macro registration.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

// Include scripts here
#include "CPPSPlayer.h"
#include "Butt.h"
#include "CPPSChaser.h"
#include "QuitScript.h"
#include "CPPSMage.h"
#include "CPPSProjectileBall.h"
#include "CPPSPlayerGolfBall.h"
#include "PauseMenuScript.h"
#include "CPPSAimUI.h"
#include "CPPGameManager.h"
#include "CPPSUpgrade.h"
#include "CPPSUpgradeManager.h"
#include "CPPSFPS.h"
#include "CPPSBlueMushroom.h"
#include "CPPSBush.h"
#include "CPPSCharger.h"
#include "CPPSSandpit.h"
#include "CPPSCameraFollow.h"
#include "CPPSBramble.h"
#include "CPPSMouse.h"
#include "CPPSMouseUI.h"
#include "CPPSMouseWorld.h"
#include "CPPShield.h"
#include "CPPSPortal.h"
#include "CPPSTutorialSubs.h"
#include "CPPSCutscene.h"
#include "CPPSDummy.h"

namespace LB 
{
	// I genuinely can't figure out why the struct method of registering is not working even though it 100% should.
	// Due to time constraints we gonna have to use this worse method of registering scripts for the time being.

	void CPPGameLogic::RegisterAll()
	{
		//CPPGameLogic::Instance()->RegisterScript<CPPSPlayer>();
		//CPPGameLogic::Instance()->RegisterScript<CPPSChaser>();
		//CPPGameLogic::Instance()->RegisterScript<Butt>();
		//CPPGameLogic::Instance()->RegisterScript<QuitScript>();
		//CPPGameLogic::Instance()->RegisterScript<CPPSMage>();
		//CPPGameLogic::Instance()->RegisterScript<CPPSBaseGolfBall>();
		//CPPGameLogic::Instance()->RegisterScript<CPPSPlayerGolfBall>();
		//CPPGameLogic::Instance()->RegisterScript<PauseMenuScript>();
		//CPPGameLogic::Instance()->RegisterScript<CPPSPlayerHUD>();
		//CPPGameLogic::Instance()->RegisterScript<CPPSPlayerHUD>();
	}
}