#include "MainMenuScene.h"



MainMenuScene::MainMenuScene() noexcept {
	m_pModel = new Model("Test.dae");
}
MainMenuScene::~MainMenuScene() noexcept {
	delete m_pModel;
}

void MainMenuScene::Render() noexcept {
	m_pModel->Render();
}