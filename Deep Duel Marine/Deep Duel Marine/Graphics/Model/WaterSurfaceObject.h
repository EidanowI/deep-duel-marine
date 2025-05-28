#pragma once
#include <string>
#include <d3d11.h>
#include <chrono>

#include "../Model/Model.h"
#include "../Shader/Shader.h"
#include "../Textures/Texture.h"


#define WATER_SURFACE_TIME_MULTIPLIER 1.6f 



class WaterSurfaceObject {
public:
	struct ConstBuf1 {
		float time[4];
	};

public:
	WaterSurfaceObject() noexcept;
	WaterSurfaceObject(const std::string& pixel_shader_name) noexcept;
	~WaterSurfaceObject() noexcept;

	void Render() noexcept;

private:
	Model* m_pModel;

	ConstBuf1 m_constBuff1;
	ID3D11Buffer* m_pConstBuff1;

	ID3D11SamplerState* m_pSamplerState;
	Texture* m_pWaterSurface_texture;
};