#pragma once

#include "LitterBox.h"
using namespace LB;

class Player {
public:
	GameObject* playerObj;
public:
	void Initialise();
	void Update();
};

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