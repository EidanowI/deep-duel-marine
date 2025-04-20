#pragma once
#include <vector>
#include <string>

#include <d3d11.h>

#include "../Utils/GraphicsMath.h"
#include "../Shader/Shader.h"
#include "../Renderer/Renderer.h"



class Mesh {
public:
	struct Vertex48B {
		Vector3D position;
		Vector3D normal;
		Vector2D UV;
		Vector4D color;
	};
	struct TriangularPolygon {
		unsigned int i1;
		unsigned int i2;
		unsigned int i3;
	};

public:
	Mesh(std::string path, VertexShader* pVertexShader);
	~Mesh();

	void Bind();
	void Render();

private:
	std::vector<Vertex48B> m_pVertexes;
	std::vector<TriangularPolygon> m_pPolygon_indexes;

	ID3D11Buffer* m_pVertex_buffer;
	ID3D11Buffer* m_pIndex_buffer;
	ID3D11InputLayout* m_pInput_layout;
};