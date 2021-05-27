#pragma once
#include "constants.h"
#include "service.h"
#include "abstractObjectService.h"
#include "playerChunkChangeEvent.h"
#include "loginEvent.h"
#include "tickEvent.h"
#include "objectCreatedEvent.h"

class ObjectService : public AbstractObjectService, public Service {
	virtual void updatePlayerChunk(PlayerChunkChangeEvent& ev);
	virtual void sendUpdates();
	virtual void sendChunkState(const std::shared_ptr<User>& user, VChunk chunk);
	std::vector<Object*> objects[29][25];
	EventObserver<PlayerChunkChangeEvent> positionChangeObserver;
	EventObserver<ObjectCreatedEvent> objectCreatedObserver;
	EventObserver<LoginEvent> loginObserver;
	EventObserver<TickEvent> tickObserver;
public:
	ObjectService(ServiceProvider* provider);
	virtual void init() override;
	virtual void interact(const std::shared_ptr<User>& user, VTile v, int objectState, const std::string& interaction, std::string objectName) override;
};
