#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <map>

#include "d3d11.h"


struct VertexShader {
	VertexShader(const std::string& name) noexcept;
	~VertexShader() noexcept;

	void Bind() noexcept;

	std::vector<char> shader_sourceCode;
	ID3D11VertexShader* pVertex_shader = nullptr;
};

struct PixelShader {
	PixelShader(const std::string& name) noexcept;
	~PixelShader() noexcept;

	void Bind() noexcept;

	std::vector<char> shader_sourceCode;
	ID3D11PixelShader* pPixel_shader = nullptr;
};

class ShaderManager {
public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static VertexShader* GetVertexShader(const std::string& name) noexcept;
	static PixelShader* GetPixelShader(const std::string& name) noexcept;

private:
	static std::map<unsigned int, VertexShader*> s_vertexShaders;
	static std::map<unsigned int, PixelShader*> s_pixelShaders;
};