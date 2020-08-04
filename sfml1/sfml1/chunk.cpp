#include "chunk.h"
#include <fstream>
#include "Tilemap.h"

Chunk::Chunk(const VChunk& pos, Measures& measures) : measures(measures), chunkpos(pos) {
    tiles = vector<vector<Tile*>>();//(64, vector<Tile*>(64, nullptr));
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
        level[x + 64 * y] = textureIndex;
        // Tile* t = new Tile(borders, {}, {}, {}, {});
        // tiles[x][y] = t;
    }
    fileName = getTexturesetFileName();
    tilemap.load(fileName, sf::Vector2u(32, 32), level, 64, 64);
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
    const float scale = measures.zoom;
    const Vector2f offset = Vector2f(32 * (64 * chunkOffset.x - relativePos.x) * scale, 32 * (64 * chunkOffset.y - relativePos.y) * scale);
    const VTile scalingDiff = measures.getInnerWindowSizeTile() * VTile(1 - scale, 1 - scale);
    const VPixel scalingDiffPx = VPixel(32 * scalingDiff.x, 32 * scalingDiff.y) / 2;
    const Vector2f finalOffset(offset.x + scalingDiffPx.x, offset.y + scalingDiffPx.y);

    Transform transform;
    const Vector2f middleOfInnerWindow(measures.getInnerWindowSizeTile().x * measures.pixelsPerTile / 2, measures.getInnerWindowSizeTile().y * measures.pixelsPerTile / 2);
    transform.scale(Vector2f(1 / measures.stretch.x, 1 / measures.stretch.y));
    transform.rotate(measures.angle, middleOfInnerWindow);
    transform.translate(finalOffset);
    transform.scale(scale, scale);
    tilemap.draw(w, transform);
}
