#pragma once
#include "../Utils/GraphicsMath.h"



struct Transform {
	void UpdateTransform() noexcept;

public:
	DirectX::XMMATRIX m_transform_matrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

	Vector3D m_position = Vector3D();
	Vector3D m_rotation = Vector3D();
	Vector3D m_scale = Vector3D(1.0f);
};