#pragma once

#include "LitterBox.h"
using namespace LB;

class SceneTestMain : public Scene
{
public:
	void Init() override;
	void Update() override;
	void Destroy() override;
};
