#include "MainMenuScene.h"



MainMenuScene::MainMenuScene() {
	m_pModel = new Model("Test.dae");
}
MainMenuScene::~MainMenuScene(){
	delete m_pModel;
}

void MainMenuScene::Render() {
	m_pModel->Render();
}