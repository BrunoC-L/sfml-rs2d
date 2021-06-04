#pragma once
#include "units.h"
#include "anchoredOffsetTransform.h"

class Component {
protected:
	const AnchoredOffsetTransform& aot;
	VPixel size;
	virtual std::pair<VPixel, VPixel> getRect();
public:
	Component(const AnchoredOffsetTransform& aot, VPixel size);
};
