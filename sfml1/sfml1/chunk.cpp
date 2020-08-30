#include "chunk.h"
#include "gameRessourceObject.h"

Chunk::Chunk(const VChunk& pos) : chunkpos(pos) {
    int* walls = new int[Measures::TilesPerChunk * Measures::TilesPerChunk];
    int* level = new int[Measures::TilesPerChunk * Measures::TilesPerChunk];
    objectmap.loadEmpty(getObjectsTexturesetFileName(), sf::Vector2u(Measures::pixelsPerTile, Measures::pixelsPerTile), new int[Measures::TilesPerChunk * Measures::TilesPerChunk], Measures::TilesPerChunk);
    tiles = vector<vector<Tile*>>(Measures::TilesPerChunk, vector<Tile*>(Measures::TilesPerChunk, nullptr));
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
        if (parameters.size() < 6)
            const int wetrrt = 2;
        if (parameters[6].length())
            const int rtert = 34;
        const auto objects        = split(parameters[6], "////");
        const auto callbacks      = split(parameters[7], "////");

        for (const auto& objStr : objects)
            new GameRessourceObject(updateObjectTexture, gameObjects, objStr);

        int absx = Measures::TilesPerChunk * chunkpos.x + x;
        int absy = Measures::TilesPerChunk * chunkpos.y + y;

        level[int(Measures::TilesPerChunk * x + y)] = textureIndex;
        walls[int(Measures::TilesPerChunk * x + y)] = borders & 0b11111;
        delete tiles[x][y];
        auto list = gameObjects[VTile(absx, absy)];
        tiles[x][y] = new Tile(absx, absy, ((borders == 16 || borders == 17 ? 15 : borders) & 0b11111) | (borders >> 5), {}, list, {}, {});
    }
    fileName = getGroundTexturesetFileName();
    tilemap  .load(fileName, sf::Vector2u(Measures::pixelsPerTile, Measures::pixelsPerTile), level, Measures::TilesPerChunk);
    fileName = getWallsTexturesetFileName();
    wallmap  .load(fileName, sf::Vector2u(Measures::pixelsPerTile, Measures::pixelsPerTile), walls, Measures::TilesPerChunk);
    file.close();
}

string Chunk::getTilesetFileName() const {
    return "../../assets/chunks/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".txt";
}

string Chunk::getGroundTexturesetFileName() const {
    return "../../assets/textures/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".png";
}


string Chunk::getObjectsTexturesetFileName() const {
    return "../../assets/textures/objects.png";
}

string Chunk::getWallsTexturesetFileName() const {
    return "../../assets/textures/walls.png";
}

Chunk::~Chunk() {
    deleted = true;
    for (int i = 0; i < tiles.size(); ++i)
        for (int j = 0; j < tiles[i].size(); ++j)
            delete tiles[i][j];
}

void Chunk::draw(RenderWindow& w, const VTile& relativePos, const VChunk& chunkOffset) {
    if (deleted)
        return;
    Transform transform = getTransform(relativePos, chunkOffset);
    tilemap  .draw(w, transform);
    wallmap  .draw(w, transform);
    objectmap.draw(w, transform);
}

Transform Chunk::getTransform(const VTile& relativePos, const VChunk& chunkOffset) const {
    auto& measures = Measures::getInstance();
    const float scale = measures.zoom;
    VTile offsetTiles = VTile(chunkOffset.x * Measures::TilesPerChunk, chunkOffset.y * Measures::TilesPerChunk) - relativePos;
    const Vector2f offset = Vector2f(Measures::pixelsPerTile * offsetTiles.x * scale, Measures::pixelsPerTile * offsetTiles.y * scale);
    const VTile scalingDiff = measures.getInnerWindowSizeTile() * VTile(1 - scale, 1 - scale);
    const VPixel scalingDiffPx = VPixel(Measures::pixelsPerTile * scalingDiff.x, Measures::pixelsPerTile * scalingDiff.y) / 2;
    const Vector2f finalOffset(offset.x + scalingDiffPx.x, offset.y + scalingDiffPx.y);

    Transform transform;
    const Vector2f middleOfInnerWindow(measures.getInnerWindowSizeTile().x * Measures::pixelsPerTile / 2, measures.getInnerWindowSizeTile().y * Measures::pixelsPerTile / 2);
    transform.scale(Vector2f(1 / measures.stretch.x, 1 / measures.stretch.y));
    transform.rotate(measures.angle, middleOfInnerWindow);
    transform.translate(finalOffset);
    transform.scale(scale, scale);
    return transform;
}
