#pragma once
#include <cstddef>
#include <iostream>

class VBase {
public:
	float x = 0, y = 0, z = 0;
	friend std::ostream& operator<<(std::ostream& s, VBase v) {
		return s << v.x << ", " << v.y;
	}

	VBase() { this->x = 0; this->y = 0; }
	VBase(float x, float y) { this->x = x; this->y = y; }
	VBase(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	VBase(const VBase& other) = default;

	VBase operator* (float scale)  const { return VBase(scale * x, scale * y); }
	VBase operator* (const VBase& scale)  const { return VBase(scale.x * x, scale.y * y); }
	VBase operator*=(const VBase& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	VBase operator*=(float scale) { x *= scale; y *= scale; z *= scale;       return *this; }

	VBase operator/ (float scale)  const { return VBase(x / scale, y / scale); }
	VBase operator/ (const VBase& scale)  const { return VBase(x / scale.x, y / scale.y); }
	VBase operator/=(const VBase& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	VBase operator/=(float scale) { x /= scale; y /= scale; z /= scale;       return *this; }

	VBase operator+ (const VBase& other)  const { return VBase(x + other.x, y + other.y); }
	VBase operator+=(const VBase& other) { x += other.x; y += other.y; z += other.z; return *this; }

	VBase operator- (const VBase& other)  const { return VBase(x - other.x, y - other.y); }
	VBase operator-=(const VBase& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

	bool  operator==(const VBase& other)  const { return x == other.x && y == other.y && z == other.z; }
	bool  operator!=(const VBase& other)  const { return x != other.x || y != other.y || z != other.z; }
};

using VPixel = VBase;
using VTile  = VBase;
using VChunk = VBase;

struct VBaseHash {
	size_t operator()(VBase const& t) const noexcept {
		return t.z + 10 * t.y + 16000 * t.x;
	}
};
