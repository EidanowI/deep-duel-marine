#pragma once
#include <string>
#include "d3d11.h"

#include "Image.h"
#include "../IBindable.h"
#include "../Renderer/Renderer.h"



class Texture : public IBindable{
public:
	Texture(std::string file_name) noexcept;
	~Texture() noexcept;

	void Bind() noexcept override;
	void Bind(int bind_index) noexcept override;

private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pShader_resource_view;
};