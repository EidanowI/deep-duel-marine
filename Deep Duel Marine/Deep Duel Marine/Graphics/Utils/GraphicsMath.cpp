#include "GraphicsMath.h"


	
float Utils::Clamp01(float value) noexcept {
	return Clamp(0.0f, 1.0f, value);
}
float Utils::Clamp(float min, float max, float value) noexcept {
	if (value > max) return max;
	if (value < min) return min;
	return value;
}
float Utils::Random01() noexcept {
	return (float(rand() % RAND_MAX)) / (float)RAND_MAX;
}
float Utils::RandomMinus11() noexcept {
	return ((float(rand() % RAND_MAX)) / (float)RAND_MAX - 0.5f) * 2.0f;
}


Vector2D::Vector2D(float x, float y) noexcept : x(x), y(y) {}
Vector2D::Vector2D(float a) noexcept : Vector2D(a, a) {}
Vector2D::Vector2D() noexcept :Vector2D(0.0f) {}

float Vector2D::Length() const noexcept {
	return sqrt(x * x + y * y);
}
Vector2D Vector2D::Normalized() const noexcept {
	float length = this->Length();
	return Vector2D(x / length, y / length);
}
void Vector2D::Normalize() noexcept {
	*this = this->Normalized();
}
Vector2D Vector2D::Clamped(float min, float max) const noexcept {
	return Vector2D(
		Utils::Clamp(min, max, x),
		Utils::Clamp(min, max, y)
	);
}
Vector2D Vector2D::Clamped01() const noexcept {
	return this->Clamped(0.0f, 1.0f);
}
void Vector2D::Clamp(float min, float max) noexcept {
	*this = this->Clamped(min, max);
}
void Vector2D::Clamp01() noexcept {
	*this = this->Clamped01();
}

Vector2D Vector2D::operator+ (const Vector2D& o) noexcept {
	return Vector2D(x + o.x, y + o.y);
}
Vector2D Vector2D::operator- (const Vector2D& o) noexcept {
	return Vector2D(x - o.x, y - o.y);
}
Vector2D Vector2D::operator* (const Vector2D& o) noexcept {
	return Vector2D(x * o.x, y * o.y);
}
Vector2D Vector2D::operator* (const float a) noexcept {
	return Vector2D(x * a, y * a);
}
Vector2D Vector2D::operator/ (const Vector2D& o) noexcept {
	return Vector2D(x / o.x, y / o.y);
}
Vector2D Vector2D::operator/ (const float a) noexcept {
	return Vector2D(x / a, y / a);
}
Vector2D Vector2D::operator+= (const Vector2D& o) noexcept {
	*this = *this + o;
	return *this;
}
Vector2D Vector2D::operator-= (const Vector2D& o) noexcept {
	*this = *this - o;
	return *this;
}
Vector2D Vector2D::operator*= (const Vector2D& o) noexcept {
	*this = *this * o;
	return *this;
}
Vector2D Vector2D::operator*= (const float a) noexcept {
	*this = *this * a;
	return *this;
}
Vector2D Vector2D::operator/= (const Vector2D& o) noexcept {
	*this = *this / o;
	return *this;
}
Vector2D Vector2D::operator/= (const float a) noexcept {
	*this = *this / a;
	return *this;
}

float& Vector2D::r() noexcept
{
	return x;
}
float& Vector2D::g() noexcept
{
	return y;
}



Vector3D::Vector3D(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
Vector3D::Vector3D(float a) noexcept : Vector3D(a, a, a) {}
Vector3D::Vector3D() noexcept :Vector3D(0.0f) {}

float Vector3D::Length() const noexcept {
	return sqrt(x * x + y * y + z * z);
}
Vector3D Vector3D::Normalized() const noexcept {
	float length = this->Length();
	return Vector3D(x / length, y / length, z / length);
}
void Vector3D::Normalize() noexcept {
	*this = this->Normalized();
}
Vector3D Vector3D::Clamped(float min, float max) const noexcept {
	return Vector3D(
		Utils::Clamp(min, max, x),
		Utils::Clamp(min, max, y),
		Utils::Clamp(min, max, z)
	);
}
Vector3D Vector3D::Clamped01() const noexcept {
	return this->Clamped(0.0f, 1.0f);
}
void Vector3D::Clamp(float min, float max) noexcept {
	*this = this->Clamped(min, max);
}
void Vector3D::Clamp01() noexcept {
	*this = this->Clamped01();
}

Vector3D Vector3D::operator+ (const Vector3D& o) noexcept {
	return Vector3D(x + o.x, y + o.y, z + o.z);
}
Vector3D Vector3D::operator- (const Vector3D& o) noexcept {
	return Vector3D(x - o.x, y - o.y, z - o.z);
}
Vector3D Vector3D::operator* (const Vector3D& o) noexcept {
	return Vector3D(x * o.x, y * o.y, z * o.z);
}
Vector3D Vector3D::operator* (const float a) noexcept {
	return Vector3D(x * a, y * a, z * a);
}
Vector3D Vector3D::operator/ (const Vector3D& o) noexcept {
	return Vector3D(x / o.x, y / o.y, z / o.z);
}
Vector3D Vector3D::operator/ (const float a) noexcept {
	return Vector3D(x / a, y / a, z / a);
}
Vector3D Vector3D::operator+= (const Vector3D& o) noexcept {
	*this = *this + o;
	return *this;
}
Vector3D Vector3D::operator-= (const Vector3D& o) noexcept {
	*this = *this - o;
	return *this;
}
Vector3D Vector3D::operator*= (const Vector3D& o) noexcept {
	*this = *this * o;
	return *this;
}
Vector3D Vector3D::operator*= (const float a) noexcept {
	*this = *this * a;
	return *this;
}
Vector3D Vector3D::operator/= (const Vector3D& o) noexcept {
	*this = *this / o;
	return *this;
}
Vector3D Vector3D::operator/= (const float a) noexcept {
	*this = *this / a;
	return *this;
}

float& Vector3D::r() noexcept
{
	return x;
}
float& Vector3D::g() noexcept
{
	return y;
}
float& Vector3D::b() noexcept
{
	return z;
}



Vector4D::Vector4D(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
Vector4D::Vector4D(float a) noexcept : Vector4D(a, a, a, a) {}
Vector4D::Vector4D() noexcept : Vector4D(0.0f) {}

Vector4D Vector4D::one() noexcept {
	return Vector4D(1.0f);
}
Vector4D Vector4D::zero() noexcept {
	return Vector4D(0.0f);
}
Vector4D Vector4D::white() noexcept {
	return Vector4D::one();
}
Vector4D Vector4D::black() noexcept {
	return Vector4D::zero();
}
Vector4D Vector4D::red() noexcept {
	return Vector4D(1.0f, 0.0f, 0.0f, 1.0f);
}
Vector4D Vector4D::green() noexcept {
	return Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
}
Vector4D Vector4D::blue() noexcept {
	return Vector4D(0.0f, 0.0f, 1.0f, 1.0f);
}
Vector4D Vector4D::yellow() noexcept {
	return Vector4D(1.0f, 1.0f, 0.0f, 1.0f);
}
Vector4D Vector4D::cyan() noexcept {
	return Vector4D(0.0f, 1.0f, 1.0f, 1.0f);
}
Vector4D Vector4D::magenta() noexcept {
	return Vector4D(1.0f, 0.0f, 1.0f, 1.0f);
}

float Vector4D::Length() const noexcept {
	return sqrt(x * x + y * y + z * z + w * w);
}
Vector4D Vector4D::Normalized() const noexcept {
	float length = this->Length();
	return Vector4D(x / length, y / length, z / length, w / length);
}
void Vector4D::Normalize() noexcept {
	*this = this->Normalized();
}
Vector4D Vector4D::Clamped(float min, float max) const noexcept {
	return Vector4D(
		Utils::Clamp(min, max, x),
		Utils::Clamp(min, max, y),
		Utils::Clamp(min, max, z),
		Utils::Clamp(min, max, w)
	);
}
Vector4D Vector4D::Clamped01() const noexcept {
	return this->Clamped(0.0f, 1.0f);
}
void Vector4D::Clamp(float min, float max) noexcept {
	*this = this->Clamped(min, max);
}
void Vector4D::Clamp01() noexcept {
	*this = this->Clamped01();
}

Vector4D Vector4D::operator+ (const Vector4D& o) noexcept {
	return Vector4D(x + o.x, y + o.y, z + o.z, w + o.w);
}
Vector4D Vector4D::operator- (const Vector4D& o) noexcept {
	return Vector4D(x - o.x, y - o.y, z - o.z, w - o.w);
}
Vector4D Vector4D::operator* (const Vector4D& o) noexcept {
	return Vector4D(x * o.x, y * o.y, z * o.z, w * o.w);
}
Vector4D Vector4D::operator* (const float a) noexcept {
	return Vector4D(x * a, y * a, z * a, w * a);
}
Vector4D Vector4D::operator/ (const Vector4D& o) noexcept {
	return Vector4D(x / o.x, y / o.y, z / o.z, w / o.w);
}
Vector4D Vector4D::operator/ (const float a) noexcept {
	return Vector4D(x / a, y / a, z / a, w / a);
}
Vector4D Vector4D::operator+= (const Vector4D& o) noexcept {
	*this = *this + o;
	return *this;
}
Vector4D Vector4D::operator-= (const Vector4D& o) noexcept {
	*this = *this - o;
	return *this;
}
Vector4D Vector4D::operator*= (const Vector4D& o) noexcept {
	*this = *this * o;
	return *this;
}
Vector4D Vector4D::operator*= (const float a) noexcept {
	*this = *this * a;
	return *this;
}
Vector4D Vector4D::operator/= (const Vector4D& o) noexcept {
	*this = *this / o;
	return *this;
}
Vector4D Vector4D::operator/= (const float a) noexcept {
	*this = *this / a;
	return *this;
}

float& Vector4D::r() noexcept
{
	return x;
}
float& Vector4D::g() noexcept
{
	return y;
}
float& Vector4D::b() noexcept
{
	return z;
}
float& Vector4D::a() noexcept
{
	return w;
}


DirectX::XMMATRIX MakeTransformMatrix(const Vector3D& position, const Vector3D& rotationRPY, const Vector3D& scale) noexcept {
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rotationRPY.x, rotationRPY.y, rotationRPY.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}
DirectX::XMMATRIX MakeTransformMatrix(const Vector3D& position, const Vector3D& rotationRPY) noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(rotationRPY.x, rotationRPY.y, rotationRPY.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}
DirectX::XMMATRIX MakeNormalMatrix(const DirectX::XMMATRIX& transform) noexcept {
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, transform));
}