#pragma once
#include <SFML/Graphics.hpp>
#include "units.h"
#include "abstractRenderWindow.h"
#include "abstractMeasures.h"

class TileMap {
public:
    int* tiles;
    unsigned sizex, sizey;
    sf::Vector2u tileSize;
    sf::VertexArray m_vertices;
    sf::Texture* m_tileset;

    bool load(sf::Texture* tileset, sf::Vector2u tileSize, int* tiles, unsigned sizex, unsigned sizey) {
        this->tileSize = tileSize;
        m_tileset = tileset;
        this->tiles = tiles;
        this->sizex = sizex;
        this->sizey = sizey;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(sizex * sizey * 4);

        for (unsigned int i = 0; i < sizex; ++i)
            for (unsigned int j = 0; j < sizey; ++j) {
                sf::Vertex* quad = &m_vertices[(i + j * sizex) * 4];

                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                int textureNumber = tiles[sizex * i + j];
                int tu = textureNumber % (m_tileset->getSize().x / tileSize.x);
                int tv = textureNumber / (m_tileset->getSize().x / tileSize.x);

                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        return true;
    }

    bool load(sf::Texture* tileset, sf::Vector2u tileSize, int* tiles, unsigned size) {
        return load(tileset, tileSize, tiles, size, size);
    }

    void update(VTile position, int newTextureIndex) {
        int i = (int)position.x % (int)AbstractMeasures::TilesPerChunk, j = (int)position.y % (int)AbstractMeasures::TilesPerChunk;
        int index = i + sizex * j;
        tiles[index] = newTextureIndex;

        int tileNumber = tiles[index];

        int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
        int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

        sf::Vertex* quad = &m_vertices[index * 4];

        quad[0].position  = sf::Vector2f( i       * tileSize.x,  j       * tileSize.y);
        quad[1].position  = sf::Vector2f((i + 1)  * tileSize.x,  j       * tileSize.y);
        quad[2].position  = sf::Vector2f((i + 1)  * tileSize.x, (j + 1)  * tileSize.y);
        quad[3].position  = sf::Vector2f( i       * tileSize.x, (j + 1)  * tileSize.y);

        quad[0].texCoords = sf::Vector2f( tu      * tileSize.x,  tv      * tileSize.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x,  tv      * tileSize.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        quad[3].texCoords = sf::Vector2f( tu      * tileSize.x, (tv + 1) * tileSize.y);
    }

    void draw(AbstractRenderWindow& w, const sf::Transform& t) const {
        sf::RenderStates states = sf::RenderStates();
        states.texture = m_tileset;
        states.transform = t;
        w.draw(m_vertices, states);
    }

    void draw(sf::RenderWindow& w, const sf::Transform& t) const {
        sf::RenderStates states = sf::RenderStates();
        states.texture = m_tileset;
        states.transform = t;
        w.draw(m_vertices, states);
    }
};
