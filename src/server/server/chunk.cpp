#include "chunk.h"

Chunk::Chunk(const VChunk& pos) : chunkpos(pos) {
    loadWalls();
}

std::string Chunk::getWallsFileName() const {
    return "../../../assets/walls/" +
        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".txt";
}

void Chunk::loadWalls() {
    std::string fileName = getWallsFileName();
    std::ifstream file(fileName);
    std::string line;
    if (!file.is_open()) {
        std::cout << "Failed to open " << fileName << std::endl;
        return;
    }

	int grid[int(TilesPerChunk) * int(TilesPerChunk)];

	if (std::getline(file, line)) {
		std::vector<std::string> content = split(line, " ");
		if (content.size() == 1) {
			int mode = stoi(content[0]);
			std::fill_n(grid, int(TilesPerChunk) * int(TilesPerChunk), mode);
		}
		else {
			const int tx = stoi(content[0]), ty = stoi(content[1]);
			grid[tx * int(TilesPerChunk) + ty] = stoi(content[2]);
		}
	}
	while (std::getline(file, line)) {
		std::vector<std::string> content = split(line, " ");
		const int tx = stoi(content[0]), ty = stoi(content[1]);
        grid[tx * int(TilesPerChunk) + ty] = stoi(content[2]);
	}
    for (int tx = 0; tx < TilesPerChunk; ++tx) {
        for (int ty = 0; ty < TilesPerChunk; ++ty) {
            int absx = TilesPerChunk * chunkpos.x + tx;
            int absy = TilesPerChunk * chunkpos.y + ty;
            tiles[tx][ty] = std::make_shared<Tile>(absx, absy, grid[tx * int(TilesPerChunk) + ty]);
        }
    }
    file.close();
}
