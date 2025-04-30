#include "MainMenuScene.h"
#include "../Renderer/Renderer.h"

struct ConstBuf1 {
	DirectX::XMMATRIX model;
};

//static ID3D11Buffer* S_pBuffer = nullptr;
//static ConstBuf1 constBuff1{};


MainMenuScene::MainMenuScene() noexcept {
	m_pCamera = new Camera(Vector3D(0, 4, 30), Vector3D(-0.2, 0, 0));

	m_pWaterSurface_object = new WaterSurfaceObject();
	//m_pModel = new Model("WaterSurfaceMainMenu.dae", "ModelVertexShader.cso", "ModelPixelShader.cso");

	//onstBuff1.model = MakeTransformMatrix(Vector3D(0.0f, 0.0f, -4.0f), Vector3D(0.0f, 0.0f, 0.0f));
	//S_pBuffer = Renderer::CreateConstBuffer((char*) & constBuff1, sizeof(ConstBuf1));
}
MainMenuScene::~MainMenuScene() noexcept {
	delete m_pCamera;
	delete m_pWaterSurface_object;
}

void MainMenuScene::Render() noexcept {
	//Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &S_pBuffer);
	m_pCamera->Bind();
	m_pWaterSurface_object->Render();
}