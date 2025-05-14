#include "WaterSurfaceObject.h"
#include "../Utils/Timer.h"


static float a = 0.0f;



WaterSurfaceObject::WaterSurfaceObject() noexcept {
	m_pModel = new Model("WaterSurfaceMainMenu.dae", "WaterSurfaceVertShader.cso", "WaterSurfacePixelShader.cso");

	m_constBuff1.time[0] = Timer::GetMsFromLastFrame() * WATER_SURFACE_TIME_MULTIPLIER;
	m_pConstBuff1 = Renderer::CreateConstBuffer((char*)&m_constBuff1, sizeof(ConstBuf1));

	m_pWaterSurface_texture = new Texture("WaterSurface.png");

	D3D11_SAMPLER_DESC samplerDesc{};
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState);
}
WaterSurfaceObject::~WaterSurfaceObject() noexcept {
	delete m_pWaterSurface_texture;

	if (m_pConstBuff1) {
		m_pConstBuff1->Release();
		m_pConstBuff1 = nullptr;
	}

	delete m_pModel;
}

void WaterSurfaceObject::Render() noexcept {
	m_constBuff1.time[0] += Timer::GetMsFromLastFrame() * WATER_SURFACE_TIME_MULTIPLIER;
	Renderer::UpdateConstBuffer(m_pConstBuff1, &m_constBuff1, sizeof(ConstBuf1));

	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1);

	Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &m_pSamplerState);
	m_pWaterSurface_texture->Bind();

	m_pModel->Render();
}