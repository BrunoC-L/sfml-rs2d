#include "units.h"

VPixel::VPixel(const float& x, const float& y)                 { this->x = x; this->y = y; }
VPixel::VPixel(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }
VPixel VPixel::operator*(const float& scale)   const { return VPixel(scale * x, scale * y); }
VPixel VPixel::operator/(const float& scale)   const { return VPixel(x / scale, y / scale); }
VPixel VPixel::operator*(const VBase& scale)   const { return VPixel(scale.x * x, scale.y * y); }
VPixel VPixel::operator/(const VBase& scale)   const { return VPixel(x / scale.x, y / scale.y); }
VPixel VPixel::operator+(const VPixel& other)  const { return VPixel(x + other.x, y + other.y); }
VPixel VPixel::operator-(const VPixel& other)  const { return VPixel(x - other.x, y - other.y); }
bool   VPixel::operator==(const VPixel& other) const { return x == other.x && y == other.y && z == other.z; }
bool   VPixel::operator!=(const VPixel& other) const { return x != other.x || y != other.y || z != other.z; }
VPixel VPixel::operator+=(const VPixel& other) { x += other.x; y += other.y; z += other.z; return *this; }
VPixel VPixel::operator-=(const VPixel& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
VPixel VPixel::operator*=(const VPixel& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
VPixel VPixel::operator/=(const VPixel& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
VPixel VPixel::operator*=(const float& scale)  { x *= scale; y *= scale; z *= scale;       return *this; }
VPixel VPixel::operator/=(const float& scale)  { x /= scale; y /= scale; z /= scale;       return *this; }

VTile::VTile(const float& x, const float& y)                 { this->x = x; this->y = y; }
VTile::VTile(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }
VTile VTile::operator* (const float& scale) const { return VTile(scale * x, scale * y); }
VTile VTile::operator/ (const float& scale) const { return VTile(x / scale, y / scale); }
VTile VTile::operator* (const VBase& scale) const { return VTile(scale.x * x, scale.y * y); }
VTile VTile::operator/ (const VBase& scale) const { return VTile(x / scale.x, y / scale.y); }
VTile VTile::operator+ (const VTile& other) const { return VTile(x + other.x, y + other.y); }
VTile VTile::operator- (const VTile& other) const { return VTile(x - other.x, y - other.y); }
bool  VTile::operator==(const VTile& other) const { return x == other.x && y == other.y && z == other.z; }
bool  VTile::operator!=(const VTile& other) const { return x != other.x || y != other.y || z != other.z; }
VTile VTile::operator+=(const VTile& other) { x += other.x; y += other.y; z += other.z; return *this; }
VTile VTile::operator-=(const VTile& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
VTile VTile::operator*=(const VTile& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
VTile VTile::operator/=(const VTile& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
VTile VTile::operator*=(const float& scale) { x *= scale; y *= scale; z *= scale;       return *this; }
VTile VTile::operator/=(const float& scale) { x /= scale; y /= scale; z /= scale;       return *this; }

VChunk::VChunk(const float& x, const float& y)                 { this->x = x; this->y = y; }
VChunk::VChunk(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }
VChunk VChunk::operator*(const float& scale)   const { return VChunk(scale * x, scale * y); }
VChunk VChunk::operator/(const float& scale)   const { return VChunk(x / scale, y / scale); }
VChunk VChunk::operator*(const VBase& scale)   const { return VChunk(scale.x * x, scale.y * y); }
VChunk VChunk::operator/(const VBase& scale)   const { return VChunk(x / scale.x, y / scale.y); }
VChunk VChunk::operator+(const VChunk& other)  const { return VChunk(x + other.x, y + other.y); }
VChunk VChunk::operator-(const VChunk& other)  const { return VChunk(x - other.x, y - other.y); }
bool   VChunk::operator==(const VChunk& other) const { return x == other.x && y == other.y && z == other.z; }
bool   VChunk::operator!=(const VChunk& other) const { return x != other.x || y != other.y || z != other.z; }
VChunk VChunk::operator+=(const VChunk& other) { x += other.x; y += other.y; z += other.z; return *this; }
VChunk VChunk::operator-=(const VChunk& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
VChunk VChunk::operator*=(const VChunk& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
VChunk VChunk::operator/=(const VChunk& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
VChunk VChunk::operator*=(const float& scale)  { x *= scale; y *= scale; z *= scale;       return *this; }
VChunk VChunk::operator/=(const float& scale)  { x /= scale; y /= scale; z /= scale;       return *this; }


size_t VTileHash::operator()(VTile const& t) const noexcept {
	return t.z + 10 * t.y + 16000 * t.x;
}
