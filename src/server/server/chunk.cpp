#include "chunk.h"

Chunk::Chunk(const VChunk& pos, ServiceProvider* provider) : chunkpos(pos) {
    this->provider = provider;
    int* walls = new int[Chunk::TilesPerChunk * Chunk::TilesPerChunk];
    int* level = new int[Chunk::TilesPerChunk * Chunk::TilesPerChunk];
    tiles = vector<vector<Tile*>>(Chunk::TilesPerChunk, vector<Tile*>(Chunk::TilesPerChunk, nullptr));
    string fileName = getTilesetFileName();
    ifstream file(fileName);
    string line;
    if (!file.is_open())
        return;
    while (getline(file, line)) {
        if (line[0] == '#')
            continue;
        vector<string> parameters = split(line, ";");
        const int x               =  stoi(parameters[0]);
        const int y               =  stoi(parameters[1]);
        const int textureIndex    =  stoi(parameters[2]);
        const int borders         =  stoi(parameters[3]);
        const auto items          = split(parameters[4], "////");
        const auto NPCs           = split(parameters[5], "////");
        const auto objects        = split(parameters[6], "////");
        const auto callbacks      = split(parameters[7], "////");

        int absx = Chunk::TilesPerChunk * chunkpos.x + x;
        int absy = Chunk::TilesPerChunk * chunkpos.y + y;

        level[int(Chunk::TilesPerChunk * x + y)] = textureIndex;
        walls[int(Chunk::TilesPerChunk * x + y)] = borders & 0b11111;
        delete tiles[x][y];
        tiles[x][y] = new Tile(absx, absy, ((borders == 16 || borders == 17 ? 15 : borders) & 0b11111) | (borders >> 5));
    }
    file.close();
}

string Chunk::getTilesetFileName() const {
    return "../../../assets/chunks/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".txt";
}

Chunk::~Chunk() {
    deleted = true;
    for (int i = 0; i < tiles.size(); ++i)
        for (int j = 0; j < tiles[i].size(); ++j)
            delete tiles[i][j];
}
