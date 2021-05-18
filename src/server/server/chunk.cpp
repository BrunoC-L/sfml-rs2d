#include "chunk.h"
#include "json.h"

Chunk::Chunk(const VChunk& pos, ObjectFactory& objectFactory) : chunkpos(pos), objectFactory(objectFactory) {
    loadWalls();
	loadObjects();
}

Chunk::~Chunk() {
	for (int i = 0; i < TilesPerChunk; ++i)
		for (int j = 0; j < TilesPerChunk; ++j)
			delete tiles[i][j];
}

std::string Chunk::getFileName(const std::string& type) const {
    return "../../../resource/chunks/" + type + "/" +
        std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".txt";
}

void Chunk::loadWalls() {
    std::string fileName = getFileName("walls");
    std::ifstream file(fileName);
	if (!file.is_open())
		throw std::exception();

	int grid[int(TilesPerChunk) * int(TilesPerChunk)];

	// the code here is terrible

	std::string line;
	if (std::getline(file, line)) {
		std::vector<std::string> content = split(line, ":");
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
		std::vector<std::string> content = split(line, ":");
		std::vector<std::string> x_y = split(content[0], "-");
		const int tx = stoi(x_y[0]), ty = stoi(x_y[1]);
        grid[tx * int(TilesPerChunk) + ty] = stoi(content[1]);
	}
    for (int tx = 0; tx < TilesPerChunk; ++tx)
        for (int ty = 0; ty < TilesPerChunk; ++ty)
            tiles[tx][ty] = new Tile(
				TilesPerChunk * chunkpos.x + tx,
				TilesPerChunk * chunkpos.y + ty,
				grid[tx * int(TilesPerChunk) + ty]
			);
    file.close();
}

void Chunk::loadObjects() {
	std::string fileName = getFileName("objects");
	std::ifstream file(fileName);
	if (!file.is_open())
		throw std::exception();
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> content = split(line, ":");
		std::vector<std::string> x_y = split(content[0], "-");
		const int tx = stoi(x_y[0]), ty = stoi(x_y[1]);
		JSON json(content[1][0] == ' ' ? content[1].substr(1) : content[1]);
		for (auto& object : json.getChildren())
			tiles[tx][ty]->addObject(objectFactory.create(object, tiles[tx][ty]));
	}
	file.close();
}
