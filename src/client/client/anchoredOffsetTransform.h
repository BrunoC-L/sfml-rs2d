#pragma once
#include "anchorTransform.h"

class AnchoredOffsetTransform : public AnchorTransform {
private:
	const AnchorTransform& anchorTransform;
	const VPixel offset;
public:
	AnchoredOffsetTransform(const AnchorTransform& anchorTransform, VPixel offset);
	virtual const sf::Transform& getTransform() const override;
	virtual VPixel getPosition() const override;
};
