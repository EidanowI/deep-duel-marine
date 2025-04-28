#include "MainMenuScene.h"
#include "../Renderer/Renderer.h"

struct ConstBuf1 {
	DirectX::XMMATRIX model;
};

static ID3D11Buffer* S_pBuffer = nullptr;

MainMenuScene::MainMenuScene() noexcept {//1.22f, 0, 0.7854
	m_pCamera = new Camera(Vector3D(75, -75, 30), Vector3D(0.7854, 1.22f, 0));

	m_pModel = new Model("Test.dae");

	ConstBuf1 constBuff1;
	constBuff1.model = MakeTransformMatrix(Vector3D(0, 0,0), Vector3D(0, 0, 0), Vector3D(1.0f));
	S_pBuffer = Renderer::CreateConstBuffer((char*) & constBuff1, sizeof(ConstBuf1));
}
MainMenuScene::~MainMenuScene() noexcept {
	delete m_pCamera;
	delete m_pModel;
}

void MainMenuScene::Render() noexcept {
	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &S_pBuffer);

	m_pCamera->Bind();
	m_pModel->Render();
}