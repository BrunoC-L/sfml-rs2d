#include "chunk.h"

std::string Chunk::getWallsFileName() const {
    return "../../../assets/walls/" +
        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".txt";
}

Chunk::Chunk(const VChunk& pos) : chunkpos(pos) {
    loadWalls();
}

void Chunk::loadWalls() {
    std::string fileName = getWallsFileName();
    std::ifstream file(fileName);
    std::string line;
    if (!file.is_open()) {
        std::cout << "Failed to open " << fileName << std::endl;
        return;
    }
    for (int x = 0; x < TilesPerChunk; ++x) {
        for (int y = 0; y < TilesPerChunk; ++y) {
            std::getline(file, line);
            const int borders = stoi(line);

            int absx = TilesPerChunk * chunkpos.x + x;
            int absy = TilesPerChunk * chunkpos.y + y;
            tiles[x][y] = std::make_shared<Tile>(absx, absy, std::min(15, borders));
        }
    }
    file.close();
}
