#pragma once
#include <string>
#include <d3d11.h>
#include <chrono>

#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"
#include "../Textures/Texture.h"



class WaterSurfaceObject {
public:
	struct ConstBuf1 {
		float time[4];
	};

public:
	WaterSurfaceObject() noexcept;
	~WaterSurfaceObject() noexcept;

	void Render() noexcept;

private:
	VertexShader* m_pVertexSahder;
	PixelShader* m_pPixelShader;

	Mesh* m_pMesh;

	ConstBuf1 m_constBuff1;
	ID3D11Buffer* m_pConstBuff1;

	ID3D11SamplerState* m_pSamplerState;
	Texture* m_pWaterSurface_texture;
};