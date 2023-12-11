#define ROWS 20
#define COLS 50
#define HUD_HEIGHT 200
#include <SFML/Graphics.hpp>
#include <iostream>
#include "hud.h"

using namespace std;

Vec2d getMouseCoords(sf::RenderWindow& window) {
    auto mousePos = sf::Mouse::getPosition(window);
    auto translatedPos = window.mapPixelToCoords(mousePos);
    return Vec2d{static_cast<unsigned int>(translatedPos.x / TILE_SIZE),
                 static_cast<unsigned int>(translatedPos.y / TILE_SIZE)};
}

int main() {
    cout << "Hola!" << endl;
    auto window = sf::RenderWindow{{TILE_SIZE * COLS, TILE_SIZE * ROWS + HUD_HEIGHT}, "Pathfinding :)"};
    window.setFramerateLimit(60);

    // create matrix
    Matrix m = Matrix(ROWS, COLS, window);
    m.setStart(Vec2d{(COLS-1) / 4, ROWS / 2});
    m.setEnd(Vec2d{COLS - COLS / 4, ROWS / 2});

    // create hud
    HUD hud = HUD(HUD_HEIGHT, TILE_SIZE * COLS, TILE_SIZE * ROWS, &m, window);

    // double click event handling
    bool leftClicked = false;
    bool rightClicked = false;
    sf::Clock leftClickClock;
    sf::Clock rightClickClock;
    const sf::Time doubleClickTime = sf::milliseconds(200); // Adjust this time as needed
    
    // hold click event handling
    bool holdLeft = false;
    bool holdRight = false;

    // popup
    bool popup_mode = true;

    // main loop
    while (window.isOpen()) {
        window.clear();
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (popup_mode) {
                if (event.type == sf::Event::KeyPressed) {
                    popup_mode = false;
                    hud.showPopup(false);
                }
            } else {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        holdLeft = true;
                        auto coords = getMouseCoords(window);
                        if (coords.x < COLS && coords.y < ROWS) {
                            if (!leftClicked || leftClickClock.getElapsedTime() > doubleClickTime) {
                                leftClicked = true;
                                leftClickClock.restart();
                                Tile* tile = m.getTile(coords.x, coords.y);
                                if (tile->type != TileType::Start && tile->type != TileType::End) {
                                    tile->changeState(TileType::Wall);
                                }
                            } else {
                                leftClicked = false;
                                m.setStart(coords);
                            }
                        }
                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        holdRight = true;
                        auto coords = getMouseCoords(window);
                        if (coords.x < COLS && coords.y < ROWS) {
                            if (!rightClicked || rightClickClock.getElapsedTime() > doubleClickTime) {
                                rightClicked = true;
                                rightClickClock.restart();
                                Tile* tile = m.getTile(coords.x, coords.y);
                                if (tile->type != TileType::Start && tile->type != TileType::End) {
                                    tile->changeState(TileType::Empty);
                                }
                            } else {
                                rightClicked = false;
                                m.setEnd(coords);
                            }
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        holdLeft = false;
                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        holdRight = false;
                    }
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::H) {
                        popup_mode = true;
                        hud.showPopup(true);
                        holdLeft = false;
                        holdRight = false;
                    } else {
                        if (event.key.code == sf::Keyboard::R) {
                            m.reset();
                            hud.resetStatus();
                        }
                        if (event.key.code == sf::Keyboard::C) {
                            m.clean();
                            hud.resetStatus();
                        }
                        if (event.key.code == sf::Keyboard::Space) {
                            hud.execute();
                        }
                        

                        if (event.key.code == sf::Keyboard::Num1) {
                            hud.changeSelector(0);
                        } else if (event.key.code == sf::Keyboard::Num2) {
                            hud.changeSelector(1);
                        } else if (event.key.code == sf::Keyboard::Num3) {
                            hud.changeSelector(2);
                        }

                        if (event.key.code == sf::Keyboard::E) {
                            hud.changeHeuristic(0);
                        } else if (event.key.code == sf::Keyboard::M) {
                            hud.changeHeuristic(1);
                        }
                    }
                }
            }
            if (holdLeft && !holdRight) {
                auto coords = getMouseCoords(window);
                if (coords.x < COLS && coords.y < ROWS) {
                    Tile* tile = m.getTile(coords.x, coords.y);
                    if (tile->type != TileType::Start && tile->type != TileType::End) {
                        tile->changeState(TileType::Wall);
                    }
                }
            }
            if (holdRight && !holdLeft) {
                auto coords = getMouseCoords(window);
                if (coords.x < COLS && coords.y < ROWS) {
                    Tile* tile = m.getTile(coords.x, coords.y);
                    if (tile->type != TileType::Start && tile->type != TileType::End) {
                        tile->changeState(TileType::Empty);
                    }
                }
            }
        }

        m.draw();
        hud.draw();
    }
    cout << "Adiós!" << endl;
}
