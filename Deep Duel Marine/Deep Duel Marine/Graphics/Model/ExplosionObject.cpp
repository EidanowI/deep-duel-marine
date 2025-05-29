#include "ExplosionObject.h"
#include "../Utils/Timer.h"



ExplosionObject::ExplosionObject() noexcept {
	m_pModel = new Model("Explosion.dae", "ExplosionVertexShader.cso", "ExplosionPixelShader.cso");

	m_constBuff1.time[0] = Timer::GetMsFromLastFrame() * 1.6f;
	m_constBuff1.model = MakeTransformMatrix(Vector3D(0.0f), Vector3D(0.0f), Vector3D(0.4f));
	m_pConstBuff1 = Renderer::CreateConstBuffer((char*)&m_constBuff1, sizeof(ConstBuf1));

	m_pExplosion_tex = new Texture("Explosion.png");

	D3D11_SAMPLER_DESC samplerDesc{};
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState);
}
ExplosionObject::~ExplosionObject() noexcept {
	delete m_pExplosion_tex;

	if (m_pConstBuff1) {
		m_pConstBuff1->Release();
		m_pConstBuff1 = nullptr;
	}

	delete m_pModel;
}

void ExplosionObject::UpdateTiming() {
	m_constBuff1.time[0] += Timer::GetMsFromLastFrame() * 4.0f;
	Renderer::UpdateConstBuffer(m_pConstBuff1, &m_constBuff1, sizeof(ConstBuf1));
}

void ExplosionObject::SetPosition(Vector3D pos, Vector3D scale) {
	m_constBuff1.model = MakeTransformMatrix(pos, Vector3D(0.0f), scale);
	Renderer::UpdateConstBuffer(m_pConstBuff1, &m_constBuff1, sizeof(ConstBuf1));
}
void ExplosionObject::Render() noexcept {
	//m_constBuff1.time[0] += Timer::GetMsFromLastFrame() * 1.6f;
	//Renderer::UpdateConstBuffer(m_pConstBuff1, &m_constBuff1, sizeof(ConstBuf1));

	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1);

	Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &m_pSamplerState);
	m_pExplosion_tex->Bind(0);

	m_pModel->Render();
}