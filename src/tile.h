#pragma once

#define TILE_SIZE 32
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

// tile types
enum TileType { Empty, Wall, WalkedOn, Path, Start, End };

void matchTexture(sf::Sprite* sprite, TileType type) {
    sf::Texture* texture = new sf::Texture();

    switch (type) {
        case Empty:
            if (!texture->loadFromFile("assets/EmptyTile.png")) {
                cout << "Error loading texture" << endl;
                sprite->setColor(sf::Color::White);
                delete texture;
            } else {
                sprite->setTexture(*texture);
            }
            break;
        case Wall:
            if (!texture->loadFromFile("assets/WallTile.png")) {
                cout << "Error loading texture" << endl;
                sprite->setColor(sf::Color::Black);
                delete texture;
            } else {
                sprite->setTexture(*texture);
            }
            break;
        case WalkedOn:
            if (!texture->loadFromFile("assets/WalkedOnTile.png")) {
                cout << "Error loading texture" << endl;
                sprite->setColor(sf::Color::Blue);
                delete texture;
            } else {
                sprite->setTexture(*texture);
            }
            break;
        case Path:
            if (!texture->loadFromFile("assets/PathTile.png")) {
                cout << "Error loading texture" << endl;
                sprite->setColor(sf::Color::Yellow);
                delete texture;
            } else {
                sprite->setTexture(*texture);
            }
            break;
        case Start:
            if (!texture->loadFromFile("assets/StartTile.png")) {
                cout << "Error loading texture" << endl;
                sprite->setColor(sf::Color::Green);
                delete texture;
            } else {
                sprite->setTexture(*texture);
            }
            break;
        case End:
            if (!texture->loadFromFile("assets/EndTile.png")) {
                cout << "Error loading texture" << endl;
                sprite->setColor(sf::Color::Red);
                delete texture;
            } else {
                sprite->setTexture(*texture);
            }
            break;
    }
}

struct Vec2d {
    unsigned int x;
    unsigned int y;

    Vec2d() : x{0}, y{0} {}
    Vec2d(unsigned int x, unsigned int y) : x{x}, y{y} {}
    Vec2d(const Vec2d& other) {
        x = other.x;
        y = other.y;
    }

    bool operator==(Vec2d& other) {
        return this->x == other.x && this->y == other.y;
    }
    bool operator!=(Vec2d& other) {
        return this->x != other.x || this->y != other.y;
    }
};

// sprite
struct Tile {
    TileType type;
    Vec2d position;
    sf::Sprite* sprite;

    Tile() : type{TileType::Empty}, position{Vec2d(0, 0)} {
        sprite = new sf::Sprite();
    }

    Tile(TileType tp, Vec2d pos) : type{tp}, position{pos} {
        sprite = new sf::Sprite();
        sprite->setPosition(pos.x * TILE_SIZE, pos.y * TILE_SIZE);
        matchTexture(sprite, type);
    }

    Tile(const Tile& other) {
        type = other.type;
        position = other.position;
        sprite = new sf::Sprite();
        sprite->setPosition(position.x * TILE_SIZE, position.y * TILE_SIZE);
        matchTexture(sprite, type);
    }

    void changeState(TileType newType) {
        this->type = newType;
        matchTexture(sprite, type);
    }

    void draw(sf::RenderWindow& window) { window.draw(*sprite); }
};
