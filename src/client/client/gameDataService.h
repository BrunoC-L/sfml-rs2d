#pragma once
#include "abstractGameDataService.h"
#include "service.h"

class GameDataService : public Service, public AbstractGameDataService {
public:
	GameDataService(AbstractServiceProvider* provider);
	void init();
};
