#include "MainMenuScene.h"
#include "../Renderer/Renderer.h"



MainMenuScene::MainMenuScene() noexcept {
	m_pCamera = new Camera(Vector3D(0, 9, 30), Vector3D(-0.2, 0, 0));

	m_pWaterSurface_object = new WaterSurfaceObject();
	
	m_pBeaconLogo_object = new BeaconLogoObject();
}
MainMenuScene::~MainMenuScene() noexcept {
	delete m_pBeaconLogo_object;
	delete m_pWaterSurface_object;
	delete m_pCamera;
}

void MainMenuScene::Render() noexcept {
	m_pCamera->Bind();

	m_pWaterSurface_object->Render();

	m_pBeaconLogo_object->Render();
}