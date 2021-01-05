#include "chunk.h"

Chunk::Chunk(const VChunk& pos, AbstractServiceProvider* provider) : chunkpos(pos) {
    this->provider = provider;
    int* walls = new int[Chunk::TilesPerChunk * Chunk::TilesPerChunk];
    int* level = new int[Chunk::TilesPerChunk * Chunk::TilesPerChunk];
    //objectmap.loadEmpty(
    //    getObjectsTexturesetFileName(),
    //    sf::Vector2u(
    //        AbstractMeasures::pixelsPerTile,
    //        AbstractMeasures::pixelsPerTile
    //    ),
    //    new int[Chunk::TilesPerChunk * Chunk::TilesPerChunk],
    //    Chunk::TilesPerChunk
    //);
    tiles = vector<vector<Tile*>>(Chunk::TilesPerChunk, vector<Tile*>(Chunk::TilesPerChunk, nullptr));
    // auto updateObjectTexture = [this](VTile position, int newTextureIndex) { if (deleted) return; this->objectmap.update(position, newTextureIndex); };
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
        if (parameters.size() < 6)
            const int wetrrt = 2;
        if (parameters[6].length())
            const int rtert = 34;
        const auto objects        = split(parameters[6], "////");
        const auto callbacks      = split(parameters[7], "////");

        //for (const auto& objStr : objects)
        //    new GameRessourceObject(updateObjectTexture, gameObjects, objStr, provider);

        int absx = Chunk::TilesPerChunk * chunkpos.x + x;
        int absy = Chunk::TilesPerChunk * chunkpos.y + y;

        level[int(Chunk::TilesPerChunk * x + y)] = textureIndex;
        walls[int(Chunk::TilesPerChunk * x + y)] = borders & 0b11111;
        delete tiles[x][y];
        // auto list = gameObjects[VTile(absx, absy)];
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
