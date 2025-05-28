#pragma once
#include <string>
#include <d3d11.h>
#include <chrono>

#include "../Model/Model.h"
#include "../Shader/Shader.h"
#include "../Textures/Texture.h"


class ShipObject {
public:
	struct ConstBuf1 {
		DirectX::XMMATRIX model;
	};

public:
	ShipObject(int ship_size, Vector3D pos) noexcept;
	~ShipObject() noexcept;

	void Render() noexcept;

private:
	Model* m_pModel;

	ConstBuf1 m_constBuff1;
	ID3D11Buffer* m_pConstBuff1;

	ID3D11SamplerState* m_pSamplerState;
	Texture* m_pShip_texture;
};
