/*!************************************************************************
 \file                Butt.cpp
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                03-11-2023
 \brief

 Source file that contains the definitions for the Butt script declared
 in Butt.h

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Butt.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Engine/Input.h"
//#include "EditorGameView.h"

namespace LB {

	/*!***********************************************************************
	\brief
	 State function gets the collider component of the game object

	 NOTE: The game object MUST HAVE the collider component, behaviour if not
	 is undefined
	*************************************************************************/
	void Butt::Start()
	{
		coll = GameObj->GetComponent<CPCollider>();
	}

	/*!***********************************************************************
	\brief
	 Update function will perform the mouse click check on the UI button and
	 perform the specified instructions.
	*************************************************************************/
	void Butt::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			auto test = COLLIDERS->OverlapCircle(EDITORGAMEVIEW->GetMousePos(), 1.f);

			for (const auto& collider : test) {
				if (coll == collider)
					DebuggerLog("UI BUTTON CLICKED");
			}
		}
	}
	/*!***********************************************************************
	\brief
	 Empty Destroy function. No purpose
	*************************************************************************/
	void Butt::Destroy()
	{
	}
}