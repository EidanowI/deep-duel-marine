#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "d3d11.h"


class VertexShader {
public:
	VertexShader(const std::string& path) noexcept;
	~VertexShader() noexcept;

private:
	std::vector<char> shader_sourceCode;
	ID3D11VertexShader* pVertex_shader = nullptr;
};