#pragma once
#include <string>
#include "d3d11.h"



class AvatarTexture{
public:
	AvatarTexture(int width, int height, const unsigned char* pDataRGBA, ID3D11Device* pDevice);
	AvatarTexture(std::string file_name, ID3D11Device* pDevice);
	~AvatarTexture();

	ID3D11ShaderResourceView* GetShaderResView();

private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pShader_resource_view;
};