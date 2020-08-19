#pragma once

class VBase {
public:
	float x = 0, y = 0, z = 0;
};

class VPixel : public VBase {
public:
	VPixel() = default;
	VPixel(const float& x, const float& y);
	VPixel(const float& x, const float& y, const float& z);
	VPixel operator*(const float& scale)   const;
	VPixel operator/(const float& scale)   const;
	VPixel operator*(const VBase& scale)   const;
	VPixel operator/(const VBase& scale)   const;
	VPixel operator+(const VPixel& other)  const;
	VPixel operator-(const VPixel& other)  const;
	bool   operator==(const VPixel& other) const;
	bool   operator!=(const VPixel& other) const;
	VPixel operator+=(const VPixel& other);
	VPixel operator-=(const VPixel& other);
	VPixel operator*=(const VPixel& other);
	VPixel operator/=(const VPixel& other);
	VPixel operator*=(const float& scale);
	VPixel operator/=(const float& scale);
};

class VTile : public VBase {
public:
	VTile() = default;
	VTile(const float& x, const float& y);
	VTile(const float& x, const float& y, const float& z);
	VTile operator* (const float& scale) const;
	VTile operator/ (const float& scale) const;
	VTile operator* (const VBase& scale) const;
	VTile operator/ (const VBase& scale) const;
	VTile operator+ (const VTile& other) const;
	VTile operator- (const VTile& other) const;
	bool  operator==(const VTile& other) const;
	bool  operator!=(const VTile& other) const;
	VTile operator+=(const VTile& other);
	VTile operator-=(const VTile& other);
	VTile operator*=(const VTile& other);
	VTile operator/=(const VTile& other);
	VTile operator*=(const float& scale);
	VTile operator/=(const float& scale);
};

class VChunk : public VBase {
public:
	VChunk() = default;
	VChunk(const float& x, const float& y);
	VChunk(const float& x, const float& y, const float& z);
	VChunk operator*(const float& scale)   const;
	VChunk operator/(const float& scale)   const;
	VChunk operator*(const VBase& scale)   const;
	VChunk operator/(const VBase& scale)   const;
	VChunk operator+(const VChunk& other)  const;
	VChunk operator-(const VChunk& other)  const;
	bool   operator==(const VChunk& other) const;
	bool   operator!=(const VChunk& other) const;
	VChunk operator+=(const VChunk& other);
	VChunk operator-=(const VChunk& other);
	VChunk operator*=(const VChunk& other);
	VChunk operator/=(const VChunk& other);
	VChunk operator*=(const float& scale);
	VChunk operator/=(const float& scale);
};

struct VTileHash
{
	size_t operator()(VTile const& t) const noexcept;
};

//struct VTileHash
//{
//	size_t operator()(VTile const& t) const noexcept
//	{
//		return t.z + 10 * t.y + 16000 * t.x;
//	}
//};