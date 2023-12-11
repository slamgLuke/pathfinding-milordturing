#pragma once

#define inf 1000000000
#include "tile.h"
#include <optional>

using namespace std;

class Matrix {
   private:
    int rows;
    int cols;
    Tile** matrix;
    Tile* start;
    Tile* end;
    bool ready;
    sf::RenderWindow& window;

   public:
    Matrix(int _rows, int _cols, sf::RenderWindow& window) : rows(_rows), cols(_cols), window(window) {
        matrix = new Tile*[cols];
        for (int x = 0; x < cols; x++) {
            matrix[x] = new Tile[rows];
            for (int y = 0; y < rows; y++) {
                Vec2d pos = Vec2d(x, y);
                matrix[x][y] = Tile(TileType::Empty, pos);
            }
        }
        start = nullptr;
        end = nullptr;
        ready = false;
    }

    ~Matrix() {
        for (int x = 0; x < cols; x++) {
            delete[] matrix[x];
        }
        delete[] matrix;
    }

    void setStart(Vec2d pos) {
        if (matrix[pos.x][pos.y].type == TileType::End) return;

        if (start) {
            matrix[start->position.x][start->position.y].changeState(TileType::Empty);
        }
        matrix[pos.x][pos.y].changeState(TileType::Start);
        start = &matrix[pos.x][pos.y];
        
        if (end) ready = true;
    }

    void setEnd(Vec2d pos) {
        if (matrix[pos.x][pos.y].type == TileType::Start) return;

        if (end) {
            matrix[end->position.x][end->position.y].changeState(TileType::Empty);
        }
        matrix[pos.x][pos.y].changeState(TileType::End);
        end = &matrix[pos.x][pos.y];

        if (start) ready = true;
    }

    void changeState(int x, int y, TileType newType) {
        matrix[x][y].changeState(newType);
        draw();
    }

    Tile* getTile(int x, int y) { return &matrix[x][y]; }
    Tile* getTile(Vec2d pos) { return &matrix[pos.x][pos.y]; }

    void clean() {
        // set all path and walkedon tiles to empty
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++)
                if (matrix[x][y].type == TileType::Path || matrix[x][y].type == TileType::WalkedOn)
                    matrix[x][y].changeState(TileType::Empty);
        }
    }

    void reset() {
        // set all tiles except start and end to empty
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++)
                if (matrix[x][y].type != TileType::Start && matrix[x][y].type != TileType::End)
                    matrix[x][y].changeState(TileType::Empty);
        }
    }

    Vec2d getStart() {
        if (start) return start->position;
        else throw std::runtime_error("Start not set");
    }

    Vec2d getEnd() {
        if (end) return end->position;
        else throw std::runtime_error("End not set");
    }

    int getRows() { return rows; }
    int getCols() { return cols; }
    bool isReady() { return ready; }

    void draw() {
        for (int x = 0; x < cols; x++) {
            for (int y = 0; y < rows; y++)
                matrix[x][y].draw(window);
        }
    }

};

