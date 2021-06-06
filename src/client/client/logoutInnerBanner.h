#pragma once
#include "innerRightBanner.h"
#include "abstractRenderWindow.h"
#include "button.h"
#include "abstractServices.h"
#include "service.h"

class LogoutInnerBanner : public InnerRightBanner, public Service {
	const std::shared_ptr<AnchorTransform> transform;
	std::shared_ptr<Button> logoutButton;
public:
	LogoutInnerBanner(ServiceProvider* provider, std::shared_ptr<AnchorTransform>&& transform);
	virtual void draw(AbstractRenderWindow* window) override;
};
