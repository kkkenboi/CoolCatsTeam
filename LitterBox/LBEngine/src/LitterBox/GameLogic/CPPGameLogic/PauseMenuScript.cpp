/*!************************************************************************
 \file				PauseMenuScript.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
 This file contains the functionality of the PauseMenuScript

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"
#include "PauseMenuScript.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Renderer/Renderer.h"
#include "CPPSMouse.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPSSettings.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPGameManager.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 Initializes the GameObjects needed for the script
	*************************************************************************/
	void PauseMenuScript::Start()
	{
		//mIsPaused = TIME->IsPaused();
		//TIME->Pause(false);

		// Grab All The GOs associated with the Pause Menu
		for (GameObject* GO : GOMANAGER->GetGameObjects())
		{
			// Pause Menu GOs
			if (GO->GetName() == "PauseMenuTextureObject")
			{
				MenuTexture = GO;
				//Renderer::GRAPHICS->swap_object_type(Renderer::Renderer_Types::RT_UI, GO->GetComponent<CPRender>());
				GO->GetComponent<CPRender>()->z_val = 0.f;
			}
			if (GO->GetName() == "PauseMenuResumeObject")
			{
				ResumeButton = GO;
			}
			if (GO->GetName() == "PauseMenuHowToPlayObject")
			{
				HowToPlayButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitObject")
			{
				QuitButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmTextureMainMenu")
			{
				BackToMainMenuTexture = GO;
			}
			if (GO->GetName() == "PauseMenuSettingsObject")
			{
				Settings = GO;
			}
			if (GO->GetName() == "PauseMenuMainMenu")
			{
				MainMenu = GO;
			}

			// How To Play GOs
			if (GO->GetName() == "PauseMenuHowToPlayTextureObject")
			{
				HowToPlayTexture = GO;
				//Renderer::GRAPHICS->swap_object_type(Renderer::Renderer_Types::RT_UI, GO->GetComponent<CPRender>());
				GO->GetComponent<CPRender>()->z_val = 0.1f;
			}
			if (GO->GetName() == "PauseMenuHowToPlayPreviousMenuButton")
			{
				HowToPlayBackButton = GO;
			}

			// Quit GOs
						// Confirm Menu
			if (GO->GetName() == "PauseMenuQuitConfirmTexture")
			{
				QuitTexture = GO;
				//Renderer::GRAPHICS->swap_object_type(Renderer::Renderer_Types::RT_UI, GO->GetComponent<CPRender>());
				GO->GetComponent<CPRender>()->z_val = 0.f;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmYesObject")
			{
				QuitYesButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmNoObject")
			{
				QuitNoButton = GO;
			}

			// Underline GO
			if (GO->GetName() == "UnderlineObject")
			{
				Underline = GO;
				GO->GetComponent<CPRender>()->z_val = -1.f;
			}

			//Settings GO
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
			}

			//sliders
			if (GO->GetName() == "SettingsMenuMV")
			{
				SettingsMenuMV = GO;
			}
			if (GO->GetName() == "SettingsMenuMVCollider")
			{
				SettingsMenuMVCollider = GO;
			}

			if (GO->GetName() == "SettingsMenuSFX")
			{
				SettingsMenuSFX = GO;
			}
			if (GO->GetName() == "SettingsMenuSFXCollider")
			{
				SettingsMenuSFXCollider = GO;
			}

			if (GO->GetName() == "SettingsMenuMusic")
			{
				SettingsMenuMusic = GO;

			}
			if (GO->GetName() == "SettingsMenuMusicCollider")
			{
				SettingsMenuMusicCollider = GO;
			}

			// Title
			if (GO->GetName() == "Title")
			{
				Title = GO;
			}

			if (GO->GetName() == "MouseCursor")
			{
				MouseCursor = GO;
			}
		}

		if (SCENEMANAGER->GetCurrentScene()->GetName() == "SceneMainMenu")
		{
			Title->GetComponent<CPAnimator>()->PlayRepeatDelay("VFX_TitleShake", 1.0f);
		}
	}

	/*!***********************************************************************
	\brief
	 Allows for button press and movement of other game objects into view
	*************************************************************************/
	void PauseMenuScript::Update()
	{
		//DEBUG->DrawCircle(INPUT->GetMousePos(), 1.f, Vec4<float>(0.f, 0.f, 1.0f, 1.0f));

		if (!TIME->IsPaused())
		{
			if (MenuTexture) {
				if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE) || INPUT->IsKeyTriggered(KeyCode::KEY_GAMEPAD_START))
				{
					if (CPPSGameManager::getGameOverTexture() && CPPSGameManager::getGameOverTexture()->IsActive())
						return;


					TIME->Pause(true);
					OnPauseMenu = true;
					// Move all the GOs to the Front of the Screen

					// To change the mouse cursor
					TIME->onPauseEvent.Invoke("Paused");

					MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
					MenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.67f, 11.50f});
					
					ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 650.f});
					ResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(334.f, 78.f, 50.f);
					
					HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 565.f});
					HowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);
					
					Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 480.f});
					Settings->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);
					
					MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 395.f});
					MainMenu->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);
					
					QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 250.f});
					QuitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(317.f, 77.f, 50.f);
				}
				else
				{
					HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				}
			}
		}

		else
		{
			if (OnPauseMenu)
			{
				if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE) || INPUT->IsKeyTriggered(KeyCode::KEY_GAMEPAD_START))
				{
					TIME->Pause(false);

					OnPauseMenu = false;

					// To change the mouse cursor
					TIME->onPauseEvent.Invoke("Playing");

					MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					//MenuTextureObject->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});

					ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					//ResumeButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

					HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					//HowToPlayButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

					HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					
					MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					//QuitButtonObject->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

					Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					BackToMainMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					SettingsMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					SettingsMenuBack->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					SettingsMenuFullscreen->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					SettingsMenuMV->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::MVSliverPos, 10000.f});
					SettingsMenuMVCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					SettingsMenuSFX->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::SFXSliderPos, 10000.f});
					SettingsMenuSFXCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					SettingsMenuMusic->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::MusicSliderPos, 10000.f});
					SettingsMenuMusicCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

					QuitTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


				}
			}
		}

		Vec2<float> mouse_pos{ INPUT->GetMousePos() };
		if (MouseCursor)
		{
			mouse_pos = MouseCursor->GetComponent<CPPSMouse>()->GetMouseUI()->GetComponent<CPTransform>()->GetPosition();
		}
		else
		{
			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		}

		std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

		// Loop through the colliders found on mouse click
		for (size_t i = 0; i < vec_colliders.size(); ++i)
		{
			// ResumeButton
			if (ResumeButton)
			{
				if (vec_colliders[i] == ResumeButton->GetComponent<CPCollider>())
				{
					if (OnPauseMenu)
					Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{941.43f, 650.f - 25.f});
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						TIME->Pause(false);

						// To change the mouse cursor
						TIME->onPauseEvent.Invoke("Playing");

						Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});

						ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						Settings->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MainMenu->GetComponent<CPCollider>()->SetWidthHeightRadius(1.f, 1.f, 1.f);

						SettingsMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						SettingsMenuBack->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						SettingsMenuFullscreen->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						SettingsMenuMV->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::MVSliverPos, 10000.f});
						SettingsMenuMVCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						SettingsMenuSFX->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::SFXSliderPos, 10000.f});
						SettingsMenuSFXCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						SettingsMenuMusic->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::MusicSliderPos, 10000.f});
						SettingsMenuMusicCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					}
				}
			}

			// HowToPlay Button
			if (HowToPlayButton)
			{
				if (vec_colliders[i] == HowToPlayButton->GetComponent<CPCollider>())
				{
					if (OnPauseMenu)
					Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{941.43f, 565.f - 20.f});
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						OnPauseMenu = false;
						Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						QuitTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						// Move all the How To Play Objects into scene
						HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{948.88f, 545.65f});
						HowToPlayTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.80f, 10.90f});

						HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{161.75f, 72.12f});
						HowToPlayBackButton->GetComponent<CPCollider>()->SetWidthHeightRadius(265.f, 222.f, 50.f);


						HowToPlayOrSettingMode = true;
					}
				}
			}

			// Setting Button
			if (Settings)
			{
				if (vec_colliders[i] == Settings->GetComponent<CPCollider>())
				{
					if (OnPauseMenu)
					Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{941.43f, 480.f - 20.f});
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						OnPauseMenu = false;
						Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						QuitTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

						SettingsMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f});
						SettingsMenuBack->GetComponent<CPTransform>()->SetPosition(Vec2<float>{955.f, 160.f});
						SettingsMenuFullscreen->GetComponent<CPTransform>()->SetPosition(Vec2<float>{1088.f, 303.f});

						SettingsMenuMV->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::MVSliverPos, 640.f});
						SettingsMenuMVCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 640.f});

						SettingsMenuSFX->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::SFXSliderPos, 510.f});
						SettingsMenuSFXCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 510.f});

						SettingsMenuMusic->GetComponent<CPTransform>()->SetPosition(Vec2<float>{CPPSSettings::MusicSliderPos, 390.f});
						SettingsMenuMusicCollider->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 390.f});

						HowToPlayOrSettingMode = true;
					}
				}
			}

			// Setting Button
			if (MainMenu)
			{
				if (vec_colliders[i] == MainMenu->GetComponent<CPCollider>())
				{
					if (OnPauseMenu)
					Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{941.43f, 395.f - 20.f});
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						//TIME->Pause(false);
						BackToMainMenu = true;
						//SCENEMANAGER->LoadScene("SceneMainMenu");
						//Test
						Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						BackToMainMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f});
						BackToMainMenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.20f, 11.10f});
						QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{715.f, 420.f});
						QuitYesButton->GetComponent<CPCollider>()->SetWidthHeightRadius(252.f, 98.f, 50.f);
						QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{1195.44f, 420.f});
						QuitNoButton->GetComponent<CPCollider>()->SetWidthHeightRadius(256.f, 91.f, 50.f);
					
						HowToPlayOrSettingMode = true;
					}
				}
			}

			// Quit Button
			if (QuitButton)
			{
				if (vec_colliders[i] == QuitButton->GetComponent<CPCollider>())
				{
					if (OnPauseMenu)
					Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{941.43f, 250.f - 10.f});
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						OnPauseMenu = false;
						BackToMainMenu = false;
						Underline->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});


						QuitTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f});
						QuitTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.20f, 11.10f});
						QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{715.f, 420.f});
						QuitYesButton->GetComponent<CPCollider>()->SetWidthHeightRadius(252.f, 98.f, 50.f);
						QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{1195.44f, 420.f});
						QuitNoButton->GetComponent<CPCollider>()->SetWidthHeightRadius(256.f, 91.f, 50.f);

						HowToPlayOrSettingMode = true;
					}
				}
			}

			if (SettingsMenuBack)
			{
				if (vec_colliders[i] == SettingsMenuBack->GetComponent<CPCollider>())
				{
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						if (!MenuTexture)
							continue;

						MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
						MenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.67f, 11.50f});

						ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 650.f});
						ResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(334.f, 78.f, 50.f);

						HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 565.f});
						HowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

						Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 480.f});
						Settings->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

						MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 395.f});
						MainMenu->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

						QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 250.f});
						QuitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(317.f, 77.f, 50.f);
					}
				}
			}

			// QuitYes Button
			if (QuitYesButton)
			{
				if (vec_colliders[i] == QuitYesButton->GetComponent<CPCollider>())
				{
					DebuggerLog("Quitting!!!!");
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						if (BackToMainMenu)
						{
							TIME->Pause(false);
							SCENEMANAGER->LoadScene("SceneMainMenu");
						}
						else
						{
							MessageQuit q;
							CORE->BroadcastMessage(&q);
						}
					}
				}
			}

			// QuitNoButton
			if (QuitNoButton)
			{
				if (vec_colliders[i] == QuitNoButton->GetComponent<CPCollider>())
				{
					DebuggerLog("Not Quitting!!!!");
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{

						OnPauseMenu = true;

						if (MenuTexture)
							MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
						if (ResumeButton)
							ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 650.f});
						if (HowToPlayButton)
							HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 565.f});
						if (QuitButton)
							QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 250.f});
						if (Settings)
							Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 480.f});
						if (MainMenu)
							MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 395.f});

						BackToMainMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
					}
				}
			}

			// HowToPlay Back Button
			if (HowToPlayBackButton)
			{
				bool _BackButtonHovered = false;
				if (vec_colliders[i] == HowToPlayBackButton->GetComponent<CPCollider>())
				{
					if (!BackButtonHovered)
					{
						HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{161.75f, 72.12f} + Vec2<float>(0, 40));
						BackButtonHovered = true;
					}
					_BackButtonHovered = true;

					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						OnPauseMenu = true;

						if (MenuTexture) {
							// If we found the block
							// Move all the Pause Menu Objects away
							// ==
							MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
							MenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.67f, 11.50f});

							ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 650.f});
							ResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(334.f, 78.f, 50.f);

							HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 565.f});
							HowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

							Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 480.f});
							Settings->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

							MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 395.f});
							MainMenu->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

							QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 250.f});
							QuitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(317.f, 77.f, 50.f);

							// Move all the How To Play Objects into scene
							HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

							HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
						}
						else {
							SCENEMANAGER->LoadScene("SceneMainMenu");
						}
					}
					break;
				}
				if (BackButtonHovered && !_BackButtonHovered)
				{
					HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{161.75f, 72.12f});
					BackButtonHovered = false;
				}
			}
		}
		if (HowToPlayOrSettingMode)
		{
			if (INPUT->IsKeyTriggered(KeyCode::KEY_ESCAPE) || INPUT->IsKeyTriggered(KeyCode::KEY_GAMEPAD_START))
			{
				if (CPPSGameManager::getGameOverTexture() && CPPSGameManager::getGameOverTexture()->IsActive())
					return;


				TIME->Pause(true);
				OnPauseMenu = true;
				//HowToPlayTexture = false;
				// Move all the GOs to the Front of the Screen

				// To change the mouse cursor
				TIME->onPauseEvent.Invoke("Paused");

				MenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.f, 529.f});
				MenuTexture->GetComponent<CPTransform>()->SetScale(Vec2<float>{19.67f, 11.50f});

				ResumeButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.66f, 650.f});
				ResumeButton->GetComponent<CPCollider>()->SetWidthHeightRadius(334.f, 78.f, 50.f);

				HowToPlayButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 565.f});
				HowToPlayButton->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

				Settings->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 480.f});
				Settings->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

				MainMenu->GetComponent<CPTransform>()->SetPosition(Vec2<float>{950.94f, 395.f});
				MainMenu->GetComponent<CPCollider>()->SetWidthHeightRadius(332.f, 74.f, 50.f);

				QuitButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{952.22f, 250.f});
				QuitButton->GetComponent<CPCollider>()->SetWidthHeightRadius(317.f, 77.f, 50.f);

				// Move all the How To Play Objects into scene
				HowToPlayTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				HowToPlayBackButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

				BackToMainMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				QuitYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});
				QuitNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.f, 10000.f});

				HowToPlayOrSettingMode = false;
			}
		}
	}

	/*!***********************************************************************
	\brief
	 Override destroy function
	*************************************************************************/
	void PauseMenuScript::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	 Getter for the OnPauseMenu member variable

	\return
	 OnPauseMenu member variable
	*************************************************************************/
	bool& PauseMenuScript::GetOnPauseMenu()
	{
		return OnPauseMenu;
	}

}