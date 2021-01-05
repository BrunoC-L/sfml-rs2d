#include "units.h"

#define VCPP(_type) \
_type::_type(const float& x, const float& y)                 { this->x = x; this->y = y; }\
_type::_type(const float& x, const float& y, const float& z) { this->x = x; this->y = y; this->z = z; }\
_type::_type(VBase other)                                    { x = other.x; y = other.y; z = other.z; }\
_type _type::operator* (const float& scale)  const { return _type(scale * x, scale * y); }\
_type _type::operator/ (const float& scale)  const { return _type(x / scale, y / scale); }\
_type _type::operator* (const VBase& scale)  const { return _type(scale.x * x, scale.y * y); }\
_type _type::operator/ (const VBase& scale)  const { return _type(x / scale.x, y / scale.y); }\
_type _type::operator+ (const _type& other)  const { return _type(x + other.x, y + other.y); }\
_type _type::operator- (const _type& other)  const { return _type(x - other.x, y - other.y); }\
bool  _type::operator==(const _type& other)  const { return x == other.x && y == other.y && z == other.z; }\
bool  _type::operator!=(const _type& other)  const { return x != other.x || y != other.y || z != other.z; }\
_type _type::operator+=(const _type& other) { x += other.x; y += other.y; z += other.z; return *this; }\
_type _type::operator-=(const _type& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }\
_type _type::operator*=(const _type& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }\
_type _type::operator/=(const _type& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }\
_type _type::operator*=(const float& scale) { x *= scale; y *= scale; z *= scale;       return *this; }\
_type _type::operator/=(const float& scale) { x /= scale; y /= scale; z /= scale;       return *this; }\

VCPP(VTile);
VCPP(VChunk);
VCPP(VPixel);

size_t VTileHash::operator()(VTile const& t) const noexcept {
	return t.z + 10 * t.y + 16000 * t.x;
}
