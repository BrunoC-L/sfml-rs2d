#pragma once
#include "constants.h"
#include "service.h"
#include "abstractObjectService.h"
#include "playerChunkChangeEvent.h"
#include "loginEvent.h"
#include "tickEvent.h"
#include "objectCreatedEvent.h"
#include "objectStateChangedEvent.h"

class ObjectService : public AbstractObjectService, public Service {
	virtual void updatePlayerChunk(const std::shared_ptr<User>& user);
	virtual void statePlayerChunk(const PlayerChunkChangeEvent::Data& ev);
	virtual void sendUpdates(const std::shared_ptr<User>& user, VChunk chunk);
	virtual void sendChunkState(const std::shared_ptr<User>& user, VChunk chunk);
	std::vector<Object*> objects[29][25];
	std::vector<Object*> changes[29][25];

	PlayerChunkChangeEvent::Observer positionChangeObserver;
	ObjectCreatedEvent::Observer objectCreatedObserver;
	LoginEvent::Observer loginObserver;
	TickEvent::Observer tickObserver;
	ObjectStateChangedEvent::Observer objectChangedObserver;
public:
	ObjectService(ServiceProvider* provider);
	virtual void init() override;
	virtual void interact(const std::shared_ptr<User>& user, VTile v, int objectState, const std::string& interaction, std::string objectName) override;
};
