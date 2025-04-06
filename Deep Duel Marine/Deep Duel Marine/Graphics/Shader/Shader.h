#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "d3d11.h"


class VertexShader {
public:
	VertexShader(const std::string& name) noexcept;
	~VertexShader() noexcept;

	void Bind() noexcept;

private:
	std::vector<char> shader_sourceCode;
	ID3D11VertexShader* pVertex_shader = nullptr;
};

class PixelShader {
public:
	PixelShader(const std::string& name) noexcept;
	~PixelShader() noexcept;

	void Bind() noexcept;

private:
	std::vector<char> shader_sourceCode;
	ID3D11PixelShader* pPixel_shader = nullptr;
};