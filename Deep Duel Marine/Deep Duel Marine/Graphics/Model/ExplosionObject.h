#pragma once
#include <string>
#include <d3d11.h>
#include <chrono>

#include "../Model/Model.h"
#include "../Shader/Shader.h"
#include "../Textures/Texture.h"

#include "../Utils/Timer.h"



class ExplosionObject {
public:
	struct ConstBuf1 {
		DirectX::XMMATRIX model;
		float time[4];
	};

public:
	ExplosionObject() noexcept;
	~ExplosionObject() noexcept;

	void UpdateTiming();
	void SetPosition(Vector3D pos, Vector3D scale);
	void Render() noexcept;

	

private:
	Model* m_pModel;

	ConstBuf1 m_constBuff1;
	ID3D11Buffer* m_pConstBuff1;

	ID3D11SamplerState* m_pSamplerState;
	Texture* m_pExplosion_tex;
};