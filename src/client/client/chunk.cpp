#include "chunk.h"
#include "session.h"

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
            TILES_PER_CHUNK * PIXELS_PER_TILE,
            TILES_PER_CHUNK * PIXELS_PER_TILE
        )
    );
    for (int x = 0; x < TILES_PER_CHUNK; ++x) {
        for (int y = 0; y < TILES_PER_CHUNK; ++y) {
            int absx = TILES_PER_CHUNK * chunkpos.x + x;
            int absy = TILES_PER_CHUNK * chunkpos.y + y;
            tiles[x][y] = std::make_shared<Tile>(absx, absy);
        }
    }
}

void Chunk::loadObjects() {
    auto x = gameData->requestObjectsForChunk(chunkpos);
    objects = x.first;
    objectMap.load(objectsTexture, sf::Vector2u(PIXELS_PER_TILE, PIXELS_PER_TILE), objects, TILES_PER_CHUNK);
    for (const auto& y : x.second)
        tiles[int(y.first.x)][int(y.first.y)]->addInteractions(y.second);
}

std::string Chunk::getGroundTexturesetFileName() const {
    return getSession().get("RS2D_HOME").asString() + "/assets/" + TEXTURES_FOLDER + "/" + CHUNKS_FOLDER + "/" +
        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".png";
}

Chunk::~Chunk() {
    delete[] objects;
}

int* Chunk::getObjectsPtr() {
    return objects;
}
#include "print.h"
void Chunk::updateInteractions(VTile tile, ObjectInteractions* interactions) {
    print("update\n");
    tiles[int(tile.x)][int(tile.y)]->addInteractions(*interactions);
}
