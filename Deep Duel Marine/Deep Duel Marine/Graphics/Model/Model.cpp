#include "Model.h"



Model::Model(std::string mesh_name) noexcept {
	m_pVertexSahder = ShaderManager::GetVertexShader("ModelVertexShader.cso");
	m_pPixelShader = ShaderManager::GetPixelShader("ModelPixelShader.cso");

	m_pMesh = new Mesh(mesh_name, m_pVertexSahder);
}
Model::~Model() noexcept {
	if (m_pMesh) {
		delete m_pMesh;
	}
}

void Model::Render() {
	Renderer::GetDeviceContext()->VSSetShader(m_pVertexSahder->pVertex_shader, nullptr, 0u);
	Renderer::GetDeviceContext()->PSSetShader(m_pPixelShader->pPixel_shader, nullptr, 0u);

	m_pMesh->Bind();
	m_pMesh->Render();
}