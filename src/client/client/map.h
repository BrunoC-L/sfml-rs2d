#pragma once
#include "units.h"
#include "item.h"
#include "abstractServices.h"
#include "service.h"
#include <thread>
#include "chunk.h"
#include "login.h"

class Map: public AbstractMap, public Service {
    std::thread updateThread;
    bool shouldStop = false;
    VChunk centerChunk;
    std::vector<std::vector<std::shared_ptr<Chunk>>> loaded;
    EventObserver<LoginEvent> loginObserver;
    EventObserver<LogoutEvent> logoutObserver;
    std::mutex mutex;
    sf::Texture objectTileset;
    bool initializing = false;
    bool isLoaded = false;
public:
    Map(ServiceProvider* provider);
    void init();
    virtual void load();
    virtual void update();
    virtual void updateChunks(const VChunk& difference, const VChunk& tempCenter);
    virtual void doUpdates();
    virtual std::shared_ptr<Tile> getTileFromVTile(VTile tilePosition);
    virtual void stopUpdates();
    virtual VChunk getCenterChunk() override;
    virtual Chunk& getLoaded(int i, int j) override;
    virtual std::mutex& getChunksMutex() override;
    virtual bool ready() override;
    virtual int* getObjectsPtrForChunk(VChunk chunk) override;
    virtual void updateInteractions(VChunk vc, VTile tile, ObjectInteractions* interactions) override;
    virtual Chunk& getChunk(VChunk chunk) override;
};
