#include "Model.h"



Model::Model(std::string mesh_name, const std::string& vertex_shader_name, const std::string& pixel_shader_name) noexcept {
	m_pVertexSahder = ShaderManager::GetVertexShader(vertex_shader_name);
	m_pPixelShader = ShaderManager::GetPixelShader(pixel_shader_name);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("Meshes/" + mesh_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (scene == nullptr ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) 
	{
		MessageBoxA(0, "Error with assimp mesh loading", "Error with assimp mesh loading", MB_ICONERROR);
		return;
	}

	for (int i = 0; i < scene->mNumMeshes; i++) {
		m_pMeshes.push_back(new Mesh(mesh_name, m_pVertexSahder, scene->mMeshes[i]));
	}
}
Model::~Model() noexcept {
	for (int i = 0; i < m_pMeshes.size(); i++) {
		delete m_pMeshes[i];
	}
}

void Model::Render() noexcept{
	Renderer::GetDeviceContext()->VSSetShader(m_pVertexSahder->pVertex_shader, nullptr, 0u);
	Renderer::GetDeviceContext()->PSSetShader(m_pPixelShader->pPixel_shader, nullptr, 0u);

	for (int i = 0; i < m_pMeshes.size(); i++) {
		m_pMeshes[i]->Bind();
		m_pMeshes[i]->Render();
	}
}