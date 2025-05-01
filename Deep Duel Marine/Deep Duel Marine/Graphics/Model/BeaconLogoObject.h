#pragma once
#include <string>
#include <d3d11.h>
#include <chrono>

#include "../Model/Model.h"
#include "../Shader/Shader.h"
#include "../Textures/Texture.h"



class BeaconLogoObject {
public:
	struct ConstBuf1 {
		DirectX::XMMATRIX model;
		float time[4];
	};

public:
	BeaconLogoObject() noexcept;
	~BeaconLogoObject() noexcept;

	void Render() noexcept;

	Vector3D UpdateAndGetAnimRot() noexcept {
		m_anim_time += 0.003f;

		if (m_anim_time >= 1.0f) {
			m_anim_time = 0;

			m_begin_key = &m_rotations[m_key_index];

			m_key_index++;

			if (m_key_index == 5) {
				m_key_index = 0;
			}
		}

		return Vector3D::Lerp(*m_begin_key, m_rotations[m_key_index], Utils::SmoothLerp01(m_anim_time));
	}

private:
	Model* m_pModel;

	ConstBuf1 m_constBuff1;
	ID3D11Buffer* m_pConstBuff1;

	ID3D11SamplerState* m_pSamplerState;
	Texture* m_pBeaconLogo_color;
	Texture* m_pBeaconLogo_blinker_mask;

	//Animation stuf
	Vector3D m_rotations[5];
	int m_key_index = 1;
	float m_anim_time = 0.0f;
	Vector3D* m_begin_key = nullptr;
	//Vector3D* m_end_key = nullptr;
};