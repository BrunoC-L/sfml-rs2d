#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"

class TileMap {
public:
    void loadEmpty(const std::string& tileset, sf::Vector2u tileSize, int* tiles, unsigned size) {
        this->tileSize = tileSize;
        this->tiles = tiles;
        this->size = size;

        m_tileset.loadFromFile(tileset);

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(size * size * 4);
    }

    bool load(const std::string& tileset, sf::Vector2u tileSize, int* tiles, unsigned size) {
        this->tileSize = tileSize;
        this->tiles = tiles;
        this->size = size;

        if (!m_tileset.loadFromFile(tileset))
            return false;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(size * size * 4);

        for (unsigned int i = 0; i < size; ++i)
            for (unsigned int j = 0; j < size; ++j) {
                sf::Vertex* quad = &m_vertices[(i + j * size) * 4];

                quad[0].position = sf::Vector2f( i      * tileSize.x,  j      * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x,  j      * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f( i      * tileSize.x, (j + 1) * tileSize.y);

                int textureNumber = tiles[size * i + j];
                int tu = textureNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = textureNumber / (m_tileset.getSize().x / tileSize.x);

                quad[0].texCoords = sf::Vector2f( tu      * tileSize.x,  tv      * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x,  tv      * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f( tu      * tileSize.x, (tv + 1) * tileSize.y);
            }
        return true;
    }

    void update(VTile position, int newTextureIndex) {
        int i = (int)position.x % (int)Measures::TilesPerChunk, j = (int)position.y % (int)Measures::TilesPerChunk;
        int index = i + size * j; // wtf
        tiles[index] = newTextureIndex;

        int tileNumber = tiles[index];

        int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
        int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

        sf::Vertex* quad = &m_vertices[index * 4];

        quad[0].position = sf::Vector2f( i      * tileSize.x,  j      * tileSize.y);
        quad[1].position = sf::Vector2f((i + 1) * tileSize.x,  j      * tileSize.y);
        quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
        quad[3].position = sf::Vector2f( i      * tileSize.x, (j + 1) * tileSize.y);

        quad[0].texCoords = sf::Vector2f( tu      * tileSize.x,  tv      * tileSize.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x,  tv      * tileSize.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        quad[3].texCoords = sf::Vector2f( tu      * tileSize.x, (tv + 1) * tileSize.y);
    }

    void draw(sf::RenderWindow& w, const sf::Transform& t) const {
        sf::RenderStates states = sf::RenderStates();
        states.texture = &m_tileset;
        states.transform = t;
        w.draw(m_vertices, states);
    }

    int* tiles;
    unsigned size;
    sf::Vector2u tileSize;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};
