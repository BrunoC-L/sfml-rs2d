#include "chunk.h"
#include "gameResourceObject.h"

Chunk::Chunk(const VChunk& pos, sf::Texture* objectsTexture, AbstractGameDataService* gameData) : 
    chunkpos(pos),
    objectsTexture(objectsTexture),
    gameData(gameData)
{
    loadTexture();
    loadObjects();
}

void Chunk::loadTexture() {
    auto file = getGroundTexturesetFileName();
    mapTexture.loadFromFile(file);
    map.setTexture(&mapTexture);
    map.setSize(
        sf::Vector2f(
            AbstractMeasures::pixelsPerTile * AbstractMeasures::TilesPerChunk,
            AbstractMeasures::pixelsPerTile * AbstractMeasures::TilesPerChunk
        )
    );
    for (int x = 0; x < AbstractMeasures::TilesPerChunk; ++x) {
        for (int y = 0; y < AbstractMeasures::TilesPerChunk; ++y) {
            int absx = AbstractMeasures::TilesPerChunk * chunkpos.x + x;
            int absy = AbstractMeasures::TilesPerChunk * chunkpos.y + y;
            tiles[x][y] = std::make_shared<Tile>(absx, absy);
        }
    }
}

void Chunk::loadObjects() {
    auto x = gameData->requestObjectsForChunk(chunkpos);
    objects = x.first;
    objectMap.load(objectsTexture, sf::Vector2u(AbstractMeasures::pixelsPerTile, AbstractMeasures::pixelsPerTile), objects, AbstractMeasures::TilesPerChunk);
    for (const auto& y : x.second) {
        std::cout << y.first.x << ", " << y.first.y << ": " << y.second.interactions.size() << std::endl;
        tiles[int(y.first.x)][int(y.first.y)]->addInteractions(y.second);
    }
}

std::string Chunk::getGroundTexturesetFileName() const {
    return "../../../assets/textures/chunks/" +
        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".png";
}

Chunk::~Chunk() {
    deleted = true;
    delete[] objects;
}

int* Chunk::getObjectsPtr() {
    return objects;
}

void Chunk::updateInteractions(VTile tile, ObjectInteractions* interactions) {
    tiles[int(tile.x)][int(tile.y)]->addInteractions(*interactions);
}
