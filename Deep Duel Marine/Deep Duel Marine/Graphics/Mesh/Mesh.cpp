#include "Mesh.h"

#include "../../Dependencies/assimp/include/assimp/Importer.hpp";
#include "../../Dependencies/assimp/include/assimp/scene.h";
#include "../../Dependencies/assimp/include/assimp/postprocess.h";



Mesh::Mesh(std::string path, VertexShader* pVertexShader) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("Meshes/" + path, aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (scene == nullptr ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		MessageBoxA(0, "Error with assimp mesh loading", "Error with assimp mesh loading", MB_ICONERROR);
		return;
	}

	if (scene->mNumMeshes != 1) {
		MessageBoxA(0, "Error num of submesh != 1", path.c_str(), MB_ICONERROR);
		return;
	}

	aiMesh* pAssimp_mesh = scene->mMeshes[0];

	m_pVertexes = std::vector<Vertex48B>(pAssimp_mesh->mNumVertices);
	m_pPolygon_indexes = std::vector<TriangularPolygon>(pAssimp_mesh->mNumFaces);

	bool is_mesh_have_uv = pAssimp_mesh->mTextureCoords[0];
	bool is_mesh_have_vertColor = pAssimp_mesh->mColors[0];

	for (unsigned int i = 0; i < pAssimp_mesh->mNumVertices; i++) {
		m_pVertexes[i].position = Vector3D(pAssimp_mesh->mVertices[i].x, pAssimp_mesh->mVertices[i].y, pAssimp_mesh->mVertices[i].z);
		m_pVertexes[i].normal = Vector3D(pAssimp_mesh->mNormals[i].x, pAssimp_mesh->mNormals[i].y, pAssimp_mesh->mNormals[i].z);
		if (is_mesh_have_uv)
		{
			m_pVertexes[i].UV = Vector2D(pAssimp_mesh->mTextureCoords[0][i].x, pAssimp_mesh->mTextureCoords[0][i].y);
		}
		else
		{
			m_pVertexes[i].UV = Vector2D(0.0f);
		}
		if (is_mesh_have_vertColor) {
			m_pVertexes[i].color = Vector4D(pAssimp_mesh->mColors[i]->r, pAssimp_mesh->mColors[i]->g, pAssimp_mesh->mColors[i]->b, pAssimp_mesh->mColors[i]->a);
		}
		else {
			m_pVertexes[i].color = Vector4D(0.0f);
		}
	}

	for (int i = 0; i < pAssimp_mesh->mNumFaces; i++) {
		m_pPolygon_indexes[i].i1 = pAssimp_mesh->mFaces[i].mIndices[0];
		m_pPolygon_indexes[i].i2 = pAssimp_mesh->mFaces[i].mIndices[1];
		m_pPolygon_indexes[i].i3 = pAssimp_mesh->mFaces[i].mIndices[2];
	}

	D3D11_BUFFER_DESC vertex_buffer_desc{};
	{
		vertex_buffer_desc.ByteWidth = m_pVertexes.size() * sizeof(Vertex48B);
		vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_IMMUTABLE can gives more fps
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.CPUAccessFlags = 0u;
		vertex_buffer_desc.MiscFlags = 0u;
		vertex_buffer_desc.StructureByteStride = sizeof(Vertex48B);
	}
	D3D11_SUBRESOURCE_DATA vertex_subRes_data{};
	{
		vertex_subRes_data.pSysMem = (void*)m_pVertexes.data();
	}
	Renderer::GetDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_subRes_data, &m_pVertex_buffer);

	D3D11_BUFFER_DESC index_buffer_desc{};
	{
		index_buffer_desc.ByteWidth = m_pPolygon_indexes.size() * sizeof(TriangularPolygon);
		index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.CPUAccessFlags = 0u;
		index_buffer_desc.MiscFlags = 0u;
		index_buffer_desc.StructureByteStride = sizeof(unsigned int);
	}
	D3D11_SUBRESOURCE_DATA index_subRes_data{};
	{
		index_subRes_data.pSysMem = (void*)m_pPolygon_indexes.data();
	}
	Renderer::GetDevice()->CreateBuffer(&index_buffer_desc, &index_subRes_data, &m_pIndex_buffer);

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[4] = {
	{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->position, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->normal, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"UV", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->UV, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"Color", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->color, D3D11_INPUT_PER_VERTEX_DATA,  0u}
	};

	Renderer::GetDevice()->CreateInputLayout(inputElementDesc,
		4u,
		pVertexShader->shader_sourceCode.data(),
		pVertexShader->shader_sourceCode.size(),
		&m_pInput_layout);
}
Mesh::~Mesh() {
	if (m_pVertex_buffer) {
		m_pVertex_buffer->Release();
		m_pVertex_buffer = nullptr;
	}

	if (m_pIndex_buffer) {
		m_pIndex_buffer->Release();
		m_pIndex_buffer = nullptr;
	}

	if (m_pInput_layout) {
		m_pInput_layout->Release();
		m_pInput_layout = nullptr;
	}
}

void Mesh::Bind() {
	unsigned int strides[] = { sizeof(Vertex48B) };
	unsigned int offsets[] = { 0u };
	Renderer::GetDeviceContext()->IASetVertexBuffers(0u, 1u, &m_pVertex_buffer, strides, offsets);
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_pIndex_buffer, DXGI_FORMAT_R16_UINT, 0u);
	Renderer::GetDeviceContext()->IASetInputLayout(m_pInput_layout);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void Mesh::Render() {
	Renderer::GetDeviceContext()->DrawIndexed(m_pPolygon_indexes.size() * 3u, 0u, 0u);
}