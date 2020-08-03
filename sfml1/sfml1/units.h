#pragma once

class VBase {
public:
	float x, y, z = 0;
};

class VPixel : public VBase {
public:
	VPixel() = default;
	VPixel(const float& x, const float& y) { this->x = x; this->y = y; }
	VPixel(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }
	VPixel operator*(const float& scale)  const { return VPixel(scale * x, scale * y); }
	VPixel operator/(const float& scale)  const { return VPixel(x / scale, y / scale); }
	VPixel operator*(const VBase& scale)  const { return VPixel(scale.x * x, scale.y * y); }
	VPixel operator/(const VBase& scale)  const { return VPixel(x / scale.x, y / scale.y); }
	VPixel operator+(const VPixel& other) const { return VPixel(x + other.x, y + other.y); }
	VPixel operator-(const VPixel& other) const { return VPixel(x - other.x, y - other.y); }
	bool   operator==(const VPixel& other) const { return x == other.x && y == other.y && z == other.z; }
	VPixel operator+=(const VPixel& other) { x += other.x; y += other.y; z += other.z; return *this; }
	VPixel operator-=(const VPixel& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	VPixel operator*=(const VPixel& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	VPixel operator/=(const VPixel& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	VPixel operator*=(const float& scale) { x *= scale; y *= scale; z *= scale; return *this; }
	VPixel operator/=(const float& scale) { x /= scale; y /= scale; z /= scale; return *this; }
};

class VTile : public VBase {
public:
	VTile() = default;
	VTile(const float& x, const float& y) { this->x = x; this->y = y; }
	VTile(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }
	VTile operator*(const float& scale) const { return VTile(scale * x, scale * y); }
	VTile operator/(const float& scale)  const { return VTile(x / scale, y / scale); }
	VTile operator*(const VBase& scale) const { return VTile(scale.x * x, scale.y * y); }
	VTile operator/(const VBase& scale)  const { return VTile(x / scale.x, y / scale.y); }
	VTile operator+(const VTile& other) const { return VTile(x + other.x, y + other.y); }
	VTile operator-(const VTile& other) const { return VTile(x - other.x, y - other.y); }
	bool  operator==(const VTile& other) const { return x == other.x && y == other.y && z == other.z; }
	VTile operator+=(const VTile& other) { x += other.x; y += other.y; z += other.z; return *this; }
	VTile operator-=(const VTile& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	VTile operator*=(const VTile& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	VTile operator/=(const VTile& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	VTile operator*=(const float& scale) { x *= scale; y *= scale; z *= scale; return *this; }
	VTile operator/=(const float& scale) { x /= scale; y /= scale; z /= scale; return *this; }
};

class VChunk : public VBase {
public:
	VChunk() = default;
	VChunk(const float& x, const float& y) { this->x = x; this->y = y; }
	VChunk(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }
	VChunk operator*(const float& scale)  const { return VChunk(scale * x, scale * y); }
	VChunk operator/(const float& scale)  const { return VChunk(x / scale, y / scale); }
	VChunk operator*(const VBase& scale)  const { return VChunk(scale.x * x, scale.y * y); }
	VChunk operator/(const VBase& scale)  const { return VChunk(x / scale.x, y / scale.y); }
	VChunk operator+(const VChunk& other) const { return VChunk(x + other.x, y + other.y); }
	VChunk operator-(const VChunk& other) const { return VChunk(x - other.x, y - other.y); }
	bool   operator==(const VChunk& other) const { return x == other.x && y == other.y && z == other.z; }
	VChunk operator+=(const VChunk& other) { x += other.x; y += other.y; z += other.z; return *this; }
	VChunk operator-=(const VChunk& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	VChunk operator*=(const VChunk& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	VChunk operator/=(const VChunk& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	VChunk operator*=(const float& scale) { x *= scale; y *= scale; z *= scale; return *this; }
	VChunk operator/=(const float& scale) { x /= scale; y /= scale; z /= scale; return *this; }
};
