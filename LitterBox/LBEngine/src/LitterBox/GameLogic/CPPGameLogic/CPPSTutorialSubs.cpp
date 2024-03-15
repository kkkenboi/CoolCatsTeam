/*!************************************************************************
 * \file				CPPSTutorialSubs.cpp
 * \author(s)			Ryan Tan Jian Hao
 * \par DP email(s):  	ryanjianhao.tan@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/03/2024
 * \brief 				This source file contains the class member function
						definition for the subtitles in the tutorial page
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSTutorialSubs.h"

//time to new letter
constexpr float ttnl{ 0.05f };
//time for viewer to read the title
constexpr float readingTime{ 1.f };
//simulated loading time
constexpr float delayTime{ 1.f };


/*!***********************************************************************
\brief
Start function to run when the script first starts. Will create all game objects
needed if not found.
*************************************************************************/
void LB::CPPSTutorialSubs::Start()
{
	titles[0] = "Well I cannot stop you from joining the tournament...";
	titles[1] = "But I can teach you how to win it!";

	mSubtitles = GOMANAGER->FindGameObjectWithName("Subtitles");
	mCurtain = GOMANAGER->FindGameObjectWithName("Transition Curtain");
	mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");

	//if object not found we need to create the object
	if (!mSubtitles)
	{
		DebuggerLogError("SUBTITLES NOT FOUND IN TUTORIAL");
		mSubtitles = FACTORY->SpawnGameObject();
		mSubtitles->AddComponent(C_CPText, FACTORY->GetCMs()[C_CPText]->Create());
	}

	if (!mCurtain)
	{
		DebuggerLogError("TRANSITION CURTAIN NOT FOUND IN TUTORIAL");
		mCurtain = FACTORY->SpawnGameObject();
	}

	mSubtitles->GetComponent<CPTransform>()->SetPosition(Vec2<float>{300.f, 540.0f});
	mCurtain->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f});
	//we put in the first subtitle here for now
	//maybe put it somewhere else in future???
	title = titles[0];
	//we limit the bound of the sub titles to the whole screen
	mSubtitles->GetComponent<CPText>()->update_msg_xbound(1920.f);
}

/*!***********************************************************************
\brief
Update function runs every frame and will advance the subtitles and remove
the transition curtain after a certain time.
*************************************************************************/
void LB::CPPSTutorialSubs::Update()
{
	//initial delays to prevent scene transition whiplash
	if (startingDelay < delayTime)
	{
		startingDelay += static_cast<float>(TIME->GetDeltaTime());
		return;
	}

	//if time passes the threshold we print the next letter
	if (timer > ttnl && currletter < title.size())
	{
		//print out the next letter in the text
		mSubtitles->GetComponent<CPText>()->update_msg_text(title.substr(
			0, static_cast<size_t>(++currletter)));
		//reset the timer
		timer = 0.f;

		return;
	}
	//increment timer
	timer += static_cast<float>(TIME->GetDeltaTime());

	//go on to next subtitle after appropriate amount of time
	if (timer > readingTime && currletter == title.size() && title != titles[1])
	{
		//once we print out the first subtitle
		//we start the next subtitle
		title = titles[1];
		currletter = 0;
		timer = 0.f;

		return;
	}

	//we raise up the curtain after the last title is played
	if (timer > readingTime && currletter == title.size() && title == titles[1] && mCurtain->GetComponent<CPTransform>()->GetPosition().y < 1650.f)
	{
		static float time{ 0.f };
		//we use quadratic to interpolate the black box up
		mCurtain->GetComponent<CPTransform>()->SetPosition(LB::Vec2<float>(960.f, 540.f + 1080.f * time * time));

		time += static_cast<float>(TIME->GetDeltaTime());
	}

	//we turn off the game object when we pass a certain threshold
	if (mCurtain->GetComponent<CPTransform>()->GetPosition().y > 1650.f)
	{
		mCurtain->SetActive(false);
	}
	if (mCurtain->GetComponent<CPTransform>()->GetPosition().y > 1000.f)
	{
		GameObj->SetActive(false);
	}
}

/*!***********************************************************************
\brief
Destroy function here to keep the compiler happy.

NOTE: I know I could just remove it but I like things in threes so don't
judge me too hard.
*************************************************************************/
void LB::CPPSTutorialSubs::Destroy()
{
}
