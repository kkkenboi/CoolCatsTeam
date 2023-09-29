/*!************************************************************************
 \file				Player.cpp
 \author(s)			Ang Jiawei Jarrett | Ryan Tan Jian Hao
 \par DP email(s):	a.jiaweijarrett@digipen.edu | ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				28-09-2023
 \brief
 This file contains the declarations for a player class. This file is a test
 file and simulates a "user created" script that utilises the engine

**************************************************************************/
#pragma once

#include "LitterBox.h"
using namespace LB;

/*!***********************************************************************
\brief
 Sample player class that a user might create
*************************************************************************/
class Player {
public:
	/*!***********************************************************************
	\brief
	GameObject of the player
	*************************************************************************/
	GameObject* playerObj;
public:
	/*!***********************************************************************
	\brief
	Initialises the player and all its components (also subscribing key events)
	*************************************************************************/
	void Initialise();
	/*!***********************************************************************
	\brief
	Update function for the player 
	*************************************************************************/
	void Update();
};


/*!***********************************************************************
\brief
Test functions for anims and movement
*************************************************************************/
void AnimUp();
void AnimLeft();
void AnimRight();
void AnimDown();

void MoveUp();
void MoveLeft();
void MoveRight();
void MoveDown(); 
void SpawnPineapples();

extern GameObject* PlayerObj;