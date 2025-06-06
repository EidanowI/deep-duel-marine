#include "BeaconLogoObject.h"
#include "../Utils/Timer.h"



BeaconLogoObject::BeaconLogoObject() noexcept {
	m_pModel = new Model("LogoBeacon.fbx", "BeaconLogoVertShader.cso", "BeaconLogoPixelShader.cso");

	m_rotations[0] = Vector3D(0.06f, -1.75f, 0.04f);
	m_rotations[1] = Vector3D(0.08f, -1.75f, 0.0f);
	m_rotations[2] = Vector3D(0.01f, -1.75f, 0.04f);
	m_rotations[3] = Vector3D(-0.03f, -1.75f, -0.01f);
	m_rotations[4] = Vector3D(0.0f, -1.75f, 0.1f);

	m_begin_key = &m_rotations[0];
	//m_end_key = &m_rotations[1];

	m_constBuff1.time[0] = Timer::GetMsFromLastFrame() * BEACON_WAVE_TIME_MULTIPLIER;
	m_constBuff1.model = MakeTransformMatrix(Vector3D(-6.5f, 0.0f, 15.0f), UpdateAndGetAnimRot(), Vector3D(2.0f));
	m_pConstBuff1 = Renderer::CreateConstBuffer((char*)&m_constBuff1, sizeof(ConstBuf1));

	m_pBeaconLogo_color = new Texture("BeaconLogoColor.png");
	m_pBeaconLogo_blinker_mask = new Texture("BeaconLogoBlinkerMask.png");

	D3D11_SAMPLER_DESC samplerDesc{};
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState);
}
BeaconLogoObject::~BeaconLogoObject() noexcept {
	delete m_pBeaconLogo_blinker_mask;
	delete m_pBeaconLogo_color;

	if (m_pConstBuff1) {
		m_pConstBuff1->Release();
		m_pConstBuff1 = nullptr;
	}

	delete m_pModel;
}

void BeaconLogoObject::Render() noexcept {
	m_constBuff1.time[0] += Timer::GetMsFromLastFrame() * BEACON_WAVE_TIME_MULTIPLIER;
	m_constBuff1.model = MakeTransformMatrix(Vector3D(-6.5f, 0.0f, 15.0f), UpdateAndGetAnimRot(), Vector3D(2.0f));
	Renderer::UpdateConstBuffer(m_pConstBuff1, &m_constBuff1, sizeof(ConstBuf1));

	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1);

	Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &m_pSamplerState);
	m_pBeaconLogo_color->Bind(0);
	m_pBeaconLogo_blinker_mask->Bind(1);

	m_pModel->Render();
}