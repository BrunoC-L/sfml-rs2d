#pragma once
#include "units.h"
#include "anchoredOffsetTransform.h"

class Component {
protected:
	const AnchorTransform& aot;
	VPixel size;
	virtual std::pair<VPixel, VPixel> getRect();
public:
	Component(const AnchorTransform& aot, VPixel size);
};
