#pragma once
#include "IScene.h"



class InGameScene : public IScene {
public:
	InGameScene() noexcept;
	~InGameScene() noexcept override;

	void Render() noexcept override;
};