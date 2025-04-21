#pragma once
#include <string>

#include "../Mesh/Mesh.h"
#include "../Shader/Shader.h"



class Model {
public:
	Model(std::string mesh_name) noexcept;
	~Model() noexcept;

	void Render();

private:
	VertexShader* m_pVertexSahder;
	PixelShader* m_pPixelShader;

	Mesh* m_pMesh;
};