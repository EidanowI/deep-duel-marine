#include "ShipObjects.h"

extern bool G_is_vertical_ship_position;



ShipObject::ShipObject(int ship_size, Vector3D pos) noexcept {
	if (ship_size == 4) {
		m_pModel = new Model("SuperMassiveShip.dae", "ModelVertexShader.cso", "ModelPixelShader.cso");
	}
	else if (ship_size == 3) {
		m_pModel = new Model("TreeShip.fbx", "ModelVertexShader.cso", "ModelPixelShader.cso");
	}
	else if (ship_size == 2) {
		m_pModel = new Model("TwoShip.fbx", "ModelVertexShader.cso", "ModelPixelShader.cso");
	}
	else {
		m_pModel = new Model("OneShip.dae", "ModelVertexShader.cso", "ModelPixelShader.cso");
	}
	float angle = G_is_vertical_ship_position ? 3.1415f : 1.57f;
	m_constBuff1.model = MakeTransformMatrix(pos, Vector3D(0, angle, 0), Vector3D(1.0f));
	m_pConstBuff1 = Renderer::CreateConstBuffer((char*)&m_constBuff1, sizeof(ConstBuf1));

	D3D11_SAMPLER_DESC samplerDesc{};
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSamplerState);
}
ShipObject::~ShipObject() noexcept {
	//delete m_pBeaconLogo_blinker_mask;

	if (m_pConstBuff1) {
		m_pConstBuff1->Release();
		m_pConstBuff1 = nullptr;
	}

	delete m_pModel;
}

void ShipObject::Render() noexcept {
	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1);

	//Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &m_pSamplerState);
	//m_pBeaconLogo_color->Bind(0);
	//m_pBeaconLogo_blinker_mask->Bind(1);

	m_pModel->Render();
}