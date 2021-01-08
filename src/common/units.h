#pragma once
#include <cstddef>

class VBase {
public:
	float x = 0, y = 0, z = 0;
};

#define VClass(_type) \
class _type : public VBase {\
public:\
	_type() = default;\
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