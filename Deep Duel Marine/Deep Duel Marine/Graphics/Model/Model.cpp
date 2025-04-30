#include "Model.h"



Model::Model(std::string mesh_name, const std::string& vertex_shader_name, const std::string& pixel_shader_name) noexcept {
	m_pVertexSahder = ShaderManager::GetVertexShader(vertex_shader_name);
	m_pPixelShader = ShaderManager::GetPixelShader(pixel_shader_name);

	m_pMesh = new Mesh(mesh_name, m_pVertexSahder);
}
Model::~Model() noexcept {
	if (m_pMesh) {
		delete m_pMesh;
	}
}

void Model::Render() noexcept{
	Renderer::GetDeviceContext()->VSSetShader(m_pVertexSahder->pVertex_shader, nullptr, 0u);
	Renderer::GetDeviceContext()->PSSetShader(m_pPixelShader->pPixel_shader, nullptr, 0u);

	m_pMesh->Bind();
	m_pMesh->Render();
}