#include "WaterSurfaceObject.h"

static float a = 0.0f;
float GetNowMilliseconds() noexcept {
	auto now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

WaterSurfaceObject::WaterSurfaceObject() noexcept {
	m_pVertexSahder = ShaderManager::GetVertexShader("WaterSurfaceVertShader.cso");
	m_pPixelShader = ShaderManager::GetPixelShader("WaterSurfacePixelShader.cso");

	m_pMesh = new Mesh("WaterSurfaceMainMenu.dae", m_pVertexSahder);

	m_constBuff1.time[0] = 0.0f;
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

	if (m_pMesh) {
		delete m_pMesh;
	}	
}

void WaterSurfaceObject::Render() noexcept {
	Renderer::GetDeviceContext()->VSSetShader(m_pVertexSahder->pVertex_shader, nullptr, 0u);
	Renderer::GetDeviceContext()->PSSetShader(m_pPixelShader->pPixel_shader, nullptr, 0u);

	m_constBuff1.time[0] += 0.01f;
	Renderer::UpdateConstBuffer(m_pConstBuff1, &m_constBuff1, sizeof(ConstBuf1));

	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1);

	Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &m_pSamplerState);
	m_pWaterSurface_texture->Bind();

	m_pMesh->Bind();
	m_pMesh->Render();
}