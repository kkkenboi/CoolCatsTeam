/*!************************************************************************
 \file                CPPSSettings.cpp
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                2-4-2024
 \brief

 Source file that contains the definition for the Settings menu member functions
 to control the settings in-game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Platform/Windows/Windows.h"
#include "LitterBox/Core/Core.h"
#include "CPPSSettings.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "PauseMenuScript.h"
#include "CPPSMouse.h"

float LB::CPPSSettings::MVSliverPos{ 1110.f };
float LB::CPPSSettings::SFXSliderPos{ 1110.f };
float LB::CPPSSettings::MusicSliderPos{ 1110.f };
constexpr float sscoords{ 1920.f };

/*!***********************************************************************
\brief
 Update function will perform the mouse click check on the UI button and
 perform the specified instructions.
*************************************************************************/
void LB::CPPSSettings::Start()
{
	coll = GameObj->GetComponent<CPCollider>();

	//get all the quit confirmation stuff
		// Grab All The GOs associated with the Pause Menu
	for (GameObject* GO : GOMANAGER->GetGameObjects())
	{
		//settings menu main portion
		if (GO->GetName() == "SettingsMenuTexture")
		{
			SettingsMenuTexture = GO;
		}
		if (GO->GetName() == "SettingsMenuBack")
		{
			SettingsMenuBack = GO;
		}

		//fullscreen tick box
		if (GO->GetName() == "SettingsMenuFullScreen")
		{
			SettingsMenuFullscreen = GO;
			SettingsMenuFullscreen->GetComponent<CPRender>()->ToggleActive(WINDOWSSYSTEM->IsFullScreen());
		}

		//sliders
		if (GO->GetName() == "SettingsMenuMV")
		{
			SettingsMenuMV = GO;
		}
		if (GO->GetName() == "SettingsMenuMVCollider")
		{
			SettingsMenuMVCollider = GO;
			width = coll->m_width;
			half_width = width * 0.45f;
			collider_left = sscoords * 0.5f - width * 0.5f;
		}

		if (GO->GetName() == "SettingsMenuSFX")
		{
			SettingsMenuSFX = GO;
		}
		if (GO->GetName() == "SettingsMenuSFXCollider")
		{
			SettingsMenuSFXCollider = GO;
			width = coll->m_width;
			half_width = width * 0.45f;
			collider_left = sscoords * 0.5f - width * 0.5f; //we use get width because every volume slider is horizontally centered.
		}

		if (GO->GetName() == "SettingsMenuMusic")
		{
			SettingsMenuMusic = GO;
		}
		if (GO->GetName() == "SettingsMenuMusicCollider")
		{
			SettingsMenuMusicCollider = GO;
			width = coll->m_width;
			half_width = width * 0.45f;
			collider_left = sscoords * 0.5f - width * 0.5f;
		}
		//for mouse collision
		if (GO->GetName() == "MouseCursor")
		{
			MouseCursor = GO;
		}
	}

	// Set pause menu pointer
	PauseMenu = GOMANAGER->FindGameObjectWithName("PauseMenuHolderObject");
}


/*!***********************************************************************
\brief
 Update function will perform the mouse click check on the UI button and
 perform the specified instructions.
*************************************************************************/
void LB::CPPSSettings::Update()
{
	//if player click in the frame
	if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
	{
		//log the click
		//DebuggerLogWarning("Mouse 1 is pressed!");
		//get the click position
		LB::Vec2<float> mouse{ INPUT->GetMousePos() };
		if (MouseCursor)
		{
			mouse = MouseCursor->GetComponent<CPPSMouse>()->GetMouseUI()->GetComponent<CPTransform>()->GetPosition();
		}
		else
		{
			mouse.y = mouse.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		}
		//get set of colliders that collides with mouse
		auto test = COLLIDERS->OverlapCircle(mouse, 1.f);

		//log the click position
		//DebuggerLogFormat("CLICK POS: %f, %f", mouse.x, mouse.y);

		//loop through colliders
		for (const auto& collider : test) {
			if (coll != collider) {
				continue;
			}

			DebuggerLogFormat("Collider Name: %s", collider->gameObj->GetName().c_str());
			
			//if player clicks on back button move the settings menu off screen
			if (GameObj->GetName() == "SettingsMenuBack") {
				//------------------------------------------Move over the quit confirmation game objects----------------------------
				if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
				{
					float go_away{ 10000.f };
					Vec2<float> hidevec{ go_away, go_away };

					SettingsMenuTexture->GetComponent<CPTransform>()->SetPosition(hidevec);
					SettingsMenuBack->GetComponent<CPTransform>()->SetPosition(hidevec);
					SettingsMenuFullscreen->GetComponent<CPTransform>()->SetPosition(hidevec);

					SettingsMenuMV->GetComponent<CPTransform>()->SetPosition(hidevec);
					SettingsMenuMVCollider->GetComponent<CPTransform>()->SetPosition(hidevec);

					SettingsMenuSFX->GetComponent<CPTransform>()->SetPosition(hidevec);
					SettingsMenuSFXCollider->GetComponent<CPTransform>()->SetPosition(hidevec);

					SettingsMenuMusic->GetComponent<CPTransform>()->SetPosition(hidevec);
					SettingsMenuMusicCollider->GetComponent<CPTransform>()->SetPosition(hidevec);

					// Set back to pause menuz
					PauseMenu->GetComponent<PauseMenuScript>()->GetOnPauseMenu() = true;
				}
				//------------------------------------------Move over the quit confirmation game objects----------------------------
			}

			//if player clicks on full screen
			//toggle the fullscreen
			if (GameObj->GetName() == "SettingsMenuFullScreen") {
				//------------------------------------------Move over the quit confirmation game objects----------------------------
				if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
				{
					WINDOWSSYSTEM->toggleFullScreen();
					SettingsMenuFullscreen->GetComponent<CPRender>()->ToggleActive(WINDOWSSYSTEM->IsFullScreen());
				}
				//------------------------------------------Move over the quit confirmation game objects----------------------------
			}
		}
	}

	//if player is holding down the mouse button then we check the slider
	if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
	{
		//same as before we map the mouse position
		LB::Vec2<float> mouse{ INPUT->GetMousePos() };
		mouse.y = mouse.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

		mouse.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		mouse.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		auto test = COLLIDERS->OverlapCircle(mouse, 1.f);

		//log the click
		//DebuggerLogFormat("CLICK POS: %f, %f", mouse.x, mouse.y);

		//loop through the objects that mouse is colliding with
		for (const auto& collider : test)
		{
			if (coll != collider) {
				continue;
			}

			float mousepos{ mouse.x };// CORE->IsEditorMode() ? mouse.x : INPUT->GetMousePos().x};

			//check which slider player is interacting with and map the location of the mouse,
			//relative to the collider size, to 0.f and 1.f for volum control
			if (GameObj->GetName() == "SettingsMenuMVCollider") {
				//------------------------------------------Move over the quit confirmation game objects----------------------------
				if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
				{
					float y{ SettingsMenuMV->GetComponent<CPTransform>()->GetPosition().y };
					SettingsMenuMV->GetComponent<CPTransform>()->SetPosition(Vec2<float>(mouse.x, y));
					MVSliverPos = SettingsMenuMV->GetComponent<CPTransform>()->GetPosition().x;
					AUDIOMANAGER->SetMasterVolume(Clamp((mousepos - collider_left) / width, 0.f, 1.f));
				}
				//------------------------------------------Move over the quit confirmation game objects----------------------------
			}
			if (GameObj->GetName() == "SettingsMenuSFXCollider") {
				//------------------------------------------Move over the quit confirmation game objects----------------------------
				if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
				{
					float y{ SettingsMenuSFX->GetComponent<CPTransform>()->GetPosition().y };
					SettingsMenuSFX->GetComponent<CPTransform>()->SetPosition(Vec2<float>(mouse.x, y));
					SFXSliderPos = SettingsMenuSFX->GetComponent<CPTransform>()->GetPosition().x;
					AUDIOMANAGER->SetChannelGroupVolume(Clamp((mousepos - collider_left) / width, 0.f, 1.f), SFX);
				}
				//------------------------------------------Move over the quit confirmation game objects----------------------------
			}
			if (GameObj->GetName() == "SettingsMenuMusicCollider") {
				//------------------------------------------Move over the quit confirmation game objects----------------------------
				if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
				{
					float y{ SettingsMenuMusic->GetComponent<CPTransform>()->GetPosition().y };
					SettingsMenuMusic->GetComponent<CPTransform>()->SetPosition(Vec2<float>(mouse.x, y));
					MusicSliderPos = SettingsMenuMusic->GetComponent<CPTransform>()->GetPosition().x;
					AUDIOMANAGER->SetChannelGroupVolume(Clamp((mousepos - collider_left) / width, 0.f, 1.f), BGM);

					DebuggerLogFormat("Volume: %f\nMouseposition: %f\ncollider_left: %f\nwidth: %f", Clamp((mousepos - collider_left) / width, 0.f, 1.f), mousepos, collider_left, width);
				}
				//------------------------------------------Move over the quit confirmation game objects----------------------------
			}
		}
	}
}
