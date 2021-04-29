#include "chunk.h"
#include "gameResourceObject.h"

Chunk::Chunk(const VChunk& pos) : chunkpos(pos) {
    loadTexture();
    //loadWalls();
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

//void Chunk::loadWalls() {
//    std::string fileName = getWallsFileName();
//    std::ifstream file(fileName);
//    std::string line;
//    if (!file.is_open()) {
//        std::cout << "Failed to open " << fileName << std::endl;
//        return;
//    }
//    for (int x = 0; x < AbstractMeasures::TilesPerChunk; ++x) {
//        for (int y = 0; y < AbstractMeasures::TilesPerChunk; ++y) {
//            std::getline(file, line);
//            const int borders = stoi(line);
//
//            int absx = AbstractMeasures::TilesPerChunk * chunkpos.x + x;
//            int absy = AbstractMeasures::TilesPerChunk * chunkpos.y + y;
//            walls[int(AbstractMeasures::TilesPerChunk * x + y)] = borders;
//            tiles[x][y] = std::make_shared<Tile>(absx, absy, std::min(15, borders));
//        }
//    }
//    fileName = getWallsTexturesetFileName();
//    wallmap.load(fileName, sf::Vector2u(AbstractMeasures::pixelsPerTile, AbstractMeasures::pixelsPerTile), walls, AbstractMeasures::TilesPerChunk);
//}

std::string Chunk::getGroundTexturesetFileName() const {
    return "../../../assets/textures/chunks/" +
        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".png";
}

//std::string Chunk::getWallsTexturesetFileName() const {
//    return "../../../assets/textures/walls.png";
//}
//
//std::string Chunk::getWallsFileName() const {
//    return "../../../assets/walls/" +
//        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".txt";
//}

Chunk::~Chunk() {
    deleted = true;
}
