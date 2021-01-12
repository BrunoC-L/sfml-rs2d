#include "chunk.h"
#include "gameRessourceObject.h"

Chunk::Chunk(const VChunk& pos, AbstractServiceProvider* provider) : chunkpos(pos) {
    this->provider = provider;
    int* walls = new int[AbstractMeasures::TilesPerChunk * AbstractMeasures::TilesPerChunk];
    int* level = new int[AbstractMeasures::TilesPerChunk * AbstractMeasures::TilesPerChunk];
    objectmap.loadEmpty(
        getObjectsTexturesetFileName(),
        sf::Vector2u(
            AbstractMeasures::pixelsPerTile,
            AbstractMeasures::pixelsPerTile
        ),
        new int[AbstractMeasures::TilesPerChunk * AbstractMeasures::TilesPerChunk],
        AbstractMeasures::TilesPerChunk
    );
    tiles = vector<vector<Tile*>>(AbstractMeasures::TilesPerChunk, vector<Tile*>(AbstractMeasures::TilesPerChunk, nullptr));
    auto updateObjectTexture = [this](VTile position, int newTextureIndex) { if (deleted) return; this->objectmap.update(position, newTextureIndex); };
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

        for (const auto& objStr : objects)
            new GameRessourceObject(updateObjectTexture, gameObjects, objStr, provider);

        int absx = AbstractMeasures::TilesPerChunk * chunkpos.x + x;
        int absy = AbstractMeasures::TilesPerChunk * chunkpos.y + y;

        level[int(AbstractMeasures::TilesPerChunk * x + y)] = textureIndex;
        walls[int(AbstractMeasures::TilesPerChunk * x + y)] = borders & 0b11111;
        delete tiles[x][y];
        auto list = gameObjects[VTile(absx, absy)];
        tiles[x][y] = new Tile(absx, absy, ((borders == 16 || borders == 17 ? 15 : borders) & 0b11111) | (borders >> 5), {}, list, {}, {});
    }
    fileName = getGroundTexturesetFileName();
    tilemap  .load(fileName, sf::Vector2u(AbstractMeasures::pixelsPerTile, AbstractMeasures::pixelsPerTile), level, AbstractMeasures::TilesPerChunk);
    fileName = getWallsTexturesetFileName();
    wallmap  .load(fileName, sf::Vector2u(AbstractMeasures::pixelsPerTile, AbstractMeasures::pixelsPerTile), walls, AbstractMeasures::TilesPerChunk);
    file.close();
}

string Chunk::getTilesetFileName() const {
    return "../../../assets/chunks/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".txt";
}

string Chunk::getGroundTexturesetFileName() const {
    return "../../../assets/textures/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".png";
}


string Chunk::getObjectsTexturesetFileName() const {
    return "../../../assets/textures/objects.png";
}

string Chunk::getWallsTexturesetFileName() const {
    return "../../../assets/textures/walls.png";
}

Chunk::~Chunk() {
    deleted = true;
    for (int i = 0; i < tiles.size(); ++i)
        for (int j = 0; j < tiles[i].size(); ++j)
            delete tiles[i][j];
}
