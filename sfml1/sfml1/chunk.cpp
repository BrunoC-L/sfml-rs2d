#include "chunk.h"
#include <fstream>
#include "Tilemap.h"

Chunk::Chunk(const VChunk& pos, Measures& measures, Textures& textures) : measures(measures), chunkpos(pos) {
    tiles = vector<vector<Tile*>>(Measures::TilesPerChunk, vector<Tile*>(Measures::TilesPerChunk, nullptr));
    string fileName = getTilesetFileName();
    ifstream file(fileName);
    string line;
    if (!file.is_open())
        return;
    while (getline(file, line)) {
        if (line[0] == '#')
            continue;
        vector<string> parameters = split(line, ';');
        const int x = stoi(parameters[0]);
        const int y = stoi(parameters[1]);
        const int textureIndex = stoi(parameters[2]);
        const int borders = stoi(parameters[3]);
        level[int(x + Measures::TilesPerChunk * y)] = textureIndex;
        walls[int(x + Measures::TilesPerChunk * y)] = borders & 0b11111;
        delete tiles[x][y];
        tiles[x][y] = new Tile(Measures::TilesPerChunk * chunkpos.x + x, Measures::TilesPerChunk * chunkpos.y + y, ((borders == 16 || borders == 17 ? 15 : borders) & 0b11111) | (borders >> 5), {}, {}, {}, {});
    }
    fileName = getTexturesetFileName();
    tilemap.load(fileName, sf::Vector2u(Measures::pixelsPerTile, Measures::pixelsPerTile), level, Measures::TilesPerChunk, Measures::TilesPerChunk);
    fileName = getWallsTexturesetFileName();
    wallmap.load(fileName, sf::Vector2u(Measures::pixelsPerTile, Measures::pixelsPerTile), walls, Measures::TilesPerChunk, Measures::TilesPerChunk);
    file.close();
}

string Chunk::getTilesetFileName() const {
    return "../../assets/chunks/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".txt";
}

string Chunk::getTexturesetFileName() const {
    return "../../assets/textures/" +
        to_string((int)chunkpos.x) + "-" + to_string((int)chunkpos.y) + "-" + to_string((int)chunkpos.z) + ".png";
}

string Chunk::getWallsTexturesetFileName() const {
    return "../../assets/wallstextures.png";
}

vector<string> Chunk::split(const string& s, const char c) const {
    vector<string> v = {};
    string buffer = "";
    for (int i = 0; i < s.length(); ++i) {
        char t = s[i];
        if (t == c) {
            v.push_back(buffer);
            buffer = "";
        }
        else
            buffer += t;
    }
    return v;
}

Chunk::~Chunk() {
    for (int i = 0; i < tiles.size(); ++i)
        for (int j = 0; j < tiles[i].size(); ++j)
            delete tiles[i][j];
}

void Chunk::draw(RenderWindow& w, const VTile& relativePos, const VChunk& chunkOffset) const {
    Transform transform = getTransform(relativePos, chunkOffset);
    tilemap.draw(w, transform);
    wallmap.draw(w, transform);
}

Transform Chunk::getTransform(const VTile& relativePos, const VChunk& chunkOffset) const {
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
