#include "chunk.h"
#include "json.h"
#include "session.h"

Chunk::Chunk(const VChunk& pos, ObjectFactory& objectFactory) : chunkpos(pos), objectFactory(objectFactory) {
    loadWalls();
	loadObjects();
}

Chunk::~Chunk() {
	for (int i = 0; i < TILES_PER_CHUNK; ++i)
		for (int j = 0; j < TILES_PER_CHUNK; ++j)
			delete tiles[i][j];
}

std::string Chunk::getFileName(const std::string& type) const {
    auto session = getSession();
	return getSession().get("RS2D_HOME").asString() + "/resource/" + CHUNKS_FOLDER + "/" + type + "/" +
		std::to_string((int)chunkpos.x) + "-" + std::to_string((int)chunkpos.y) + "-" + std::to_string((int)chunkpos.z) + ".txt";
}

std::string Chunk::getObjectFileNameWithPath(const std::string& objectFileName) const {
	return  getSession().get("RS2D_HOME").asString() + "/resource/objects/list/" + objectFileName + "/object.json";
}

void Chunk::loadWalls() {
    std::string fileName = getFileName("walls");
    std::ifstream file(fileName);
	if (!file.is_open())
		throw std::runtime_error("Couldn't open file: " + fileName);

	int* grid = new int[int(TILES_PER_CHUNK) * int(TILES_PER_CHUNK)]();

	// the code here is terrible

	std::string line;
	if (std::getline(file, line)) {
		std::vector<std::string> content = split(line, ":");
		if (content.size() == 1) {
			int mode = stoi(content[0]);
			std::fill_n(grid, int(TILES_PER_CHUNK) * int(TILES_PER_CHUNK), mode);
		}
		else {
			auto x_y = split(content[0], "-");
			int tx = stoi(x_y[0]);
			int ty = stoi(x_y[1]);
			grid[tx * int(TILES_PER_CHUNK) + ty] = stoi(content[1]);
		}
	}
	while (std::getline(file, line)) {
		std::vector<std::string> content = split(line, ":");
		std::vector<std::string> x_y = split(content[0], "-");
		int tx = stoi(x_y[0]);
		int ty = stoi(x_y[1]);
        grid[tx * int(TILES_PER_CHUNK) + ty] = stoi(content[1]);
	}
    for (int tx = 0; tx < TILES_PER_CHUNK; ++tx)
        for (int ty = 0; ty < TILES_PER_CHUNK; ++ty)
            tiles[tx][ty] = new Tile(
				int(TILES_PER_CHUNK * chunkpos.x + tx),
				int(TILES_PER_CHUNK * chunkpos.y + ty),
				grid[tx * int(TILES_PER_CHUNK) + ty]
			);
    file.close();
}

void Chunk::loadObjects() {
	std::string fileName = getFileName("objects");
	std::ifstream file(fileName);
	if (!file.is_open())
		throw std::runtime_error("Couldn't open file: " + fileName);
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> content = split(line, ":");
		std::vector<std::string> x_y = split(content[0], "-");
		const int tx = stoi(x_y[0]), ty = stoi(x_y[1]);
		JSON json(content[1][0] == ' ' ? content[1].substr(1) : content[1]);
		for (auto& objectFileNameJSON : json.getChildren()) {
			std::string objectName = objectFileNameJSON.asString();
			std::string objectFileName = getObjectFileNameWithPath(objectName);
			std::ifstream objectFile(objectFileName);
			if (!objectFile.is_open())
				throw std::runtime_error("Couldn't open file: " + fileName);
			std::string fileStr;
			std::istreambuf_iterator<char> inputIt(objectFile), emptyInputIt;
			std::back_insert_iterator<std::string> stringInsert(fileStr);
			copy(inputIt, emptyInputIt, stringInsert);
			tiles[tx][ty]->addObject(objectFactory.create(objectName, fileStr, tiles[tx][ty]));
		}
	}
	file.close();
}
