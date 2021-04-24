#pragma once
#include "abstractMap.h"
#include "service.h"
#include <unordered_map>

class Map : public AbstractMap, public Service {
	vector<Chunk*> chunks;
public:
	Map(ServiceProvider* provider);
	virtual void init();
	virtual void load();
	virtual Chunk* getChunk(VChunk v);
};
