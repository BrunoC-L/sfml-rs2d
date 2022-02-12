#pragma once
#include "abstractGameDataService.h"
#include "GameTickProgress.h"
#include "service.h"
#include "PlayerPositions.h"
#include "login.h"

class GameDataService : public Service, public AbstractGameDataService {
	bool loggedIn = false;
	GameTickProgress* tracker;
	std::unique_ptr<PlayerPositions> playerPositions;
	LoginEvent::Observer loginObserver;
	LogoutEvent::Observer logoutObserver;
	std::vector<std::pair<VChunk, std::pair<int*, std::vector<std::pair<VTile, ObjectInteractions>>>>> objectsReceived;
	void storePositions(const JSON& json);
	std::vector<std::pair<VTile, std::pair<int, ObjectInteractions>>> parseObject(const JSON& object, VChunk chunk);
	std::unordered_map<std::string, int> name2texture;
public:
	GameDataService(ServiceProvider* provider, GameTickProgress* tracker);
	virtual void init();

	virtual const bool& userIsLoggedIn() override;
	virtual std::vector<playerIdAndPosition> getPlayerPositions() override;
	virtual std::pair<int*, std::vector<std::pair<VTile, ObjectInteractions>>> requestObjectsForChunk(VChunk chunk) override;
	virtual void clearObjectsCache() override;
};
