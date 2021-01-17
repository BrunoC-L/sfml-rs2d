#pragma once
#include <cstddef>
#include <iostream>

class VBase {
public:
	float x = 0, y = 0, z = 0;
	friend std::ostream& operator<<(std::ostream& s, VBase v) {
		return s << v.x << ", " << v.y;
	}
};

#define VClass(_type) \
class _type : public VBase {\
public:\
	_type();\
	_type(const float& x, const float& y);\
	_type(const float& x, const float& y, const float& z);\
	_type(VBase other);\
	_type operator* (const float& scale)  const;\
	_type operator/ (const float& scale)  const;\
	_type operator* (const VBase& scale)  const;\
	_type operator/ (const VBase& scale)  const;\
	_type operator+ (const _type& other)  const;\
	_type operator- (const _type& other)  const;\
	bool  operator==(const _type& other)  const;\
	bool  operator!=(const _type& other)  const;\
	_type operator+=(const _type& other);\
	_type operator-=(const _type& other);\
	_type operator*=(const _type& other);\
	_type operator/=(const _type& other);\
	_type operator*=(const float& scale);\
	_type operator/=(const float& scale);\
};\

VClass(VPixel);
VClass(VTile);
VClass(VChunk);

struct VTileHash
{
	size_t operator()(VTile const& t) const noexcept;
};
