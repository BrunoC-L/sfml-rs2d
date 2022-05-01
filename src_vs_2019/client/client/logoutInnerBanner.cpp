#include "logoutInnerBanner.h"
#include "textures.h"

LogoutInnerBanner::LogoutInnerBanner(ServiceProvider* provider, std::shared_ptr<AnchorTransform>&& transform) : transform(transform), Service(provider) {
	acquire();
	logoutButton = std::make_shared<Button>(std::make_shared<AnchoredOffsetTransform>(*this->transform, VPixel(50, 100)), VPixel(145, 70));
	logoutButton->texture(logoutTabButtonTexture);
	logoutButton->onClick([&]() {
		socket->disconnect();
	});
}

void LogoutInnerBanner::draw(AbstractRenderWindow* window) {
	logoutButton->draw(*window);
}
