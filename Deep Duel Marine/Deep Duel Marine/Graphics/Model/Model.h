#pragma once
#include <string>

#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"



class Model {
public:
	Model(std::string mesh_name, const std::string& vertex_shader_name, const std::string& pixel_shader_name) noexcept;
	~Model() noexcept;

	void Render() noexcept;

private:
	VertexShader* m_pVertexSahder;
	PixelShader* m_pPixelShader;

	Mesh* m_pMesh;
};