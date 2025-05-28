#pragma once
#include "IScene.h"
#include "../Model/Model.h"
#include "../Camera/Camera.h"
#include "../Model/WaterSurfaceObject.h"
#include "../Model/InGameObjects.h"



class InGameScene : public IScene {
public:
	InGameScene() noexcept;
	~InGameScene() noexcept override;

	void Render() noexcept override;
private:
	Camera* m_pCamera;
	WaterSurfaceObject* m_pWaterSurface_object;
	FieldHintObject* m_pFieldHint_object;
};