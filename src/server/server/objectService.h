#pragma once
#include "service.h"
#include <string>
#include "constants.h"
#include "playerChunkChangeEvent.h"
#include "loginEvent.h"
#include "tickEvent.h"
#include "objectCreatedEvent.h"
#include "objectStateChangedEvent.h"

class ObjectService : public Service {
	virtual void updatePlayerChunk(const std::shared_ptr<User>& user);
	virtual void statePlayerChunk(PlayerChunkChangeEvent& ev);
	virtual void sendUpdates(const std::shared_ptr<User>& user, VChunk chunk);
	virtual void sendChunkState(const std::shared_ptr<User>& user, VChunk chunk);
	std::vector<Object*> objects[29][25];
	std::vector<Object*> changes[29][25];

	EventObserver<PlayerChunkChangeEvent> positionChangeObserver;
	EventObserver<ObjectCreatedEvent> objectCreatedObserver;
	EventObserver<LoginEvent> loginObserver;
	EventObserver<TickEvent> tickObserver;
	EventObserver<ObjectStateChangedEvent> objectChangedObserver;
public:
	ObjectService(ServiceProvider* provider);
	virtual void init();
	virtual void interact(const std::shared_ptr<User>& user, VTile v, int objectState, const std::string& interaction, std::string objectName);
};
