#pragma once

#include "functions.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class HUD {
private:
  int height;
  int width;
  int top;
  Matrix *matrix;
  int selector;
  int h_selector;
  sf::RenderWindow &window;

  sf::RectangleShape bg;
  sf::RectangleShape slider;
  sf::RectangleShape slider_bg;

    sf::Font font;
    string instructions = "Press SPACE to run the simulation!\nH for help";
    sf::Text t1;
    string status = "Waiting for input";
    sf::Text s;
    string heuristic = "";
    sf::Text h;
    string* s_algorithms;
    sf::Text* t_algorithms;

    bool show_popup = true;
    string popup = "Use left click to place walls, right click to remove walls.\nUse double left click to place the start.\nDouble right click to place the end.\n\nPress R to reset the grid (deletes all walls).\nC to clean the algorithm trace.\n\nUse numbers 1-2-3 to select the algorithm.\nUse keys E and M to change the heuristic.\n\nPress SPACE to run the simulation.\n\n\nPress any key to close this popup...";
    sf::Text p;
    sf::RectangleShape p_bg;

   public:
    HUD(int height,
        int width,
        int top,
        Matrix* matrix,
        sf::RenderWindow& window)
        : height(height),
          width(width),
          top(top),
          matrix(matrix),
          window(window) {
        selector = 0;
        h_selector = 0;

    // bg
    bg = sf::RectangleShape(sf::Vector2f(width, height));
    bg.setFillColor(sf::Color(0, 0, 80, 255));
    bg.setPosition(0, top);

    // selector
    slider_bg = sf::RectangleShape(sf::Vector2f((width - 20), 50));
    slider_bg.setFillColor(sf::Color(0, 180, 0, 255));
    slider_bg.setPosition(10, top + 140);

    slider = sf::RectangleShape(sf::Vector2f((width - 20) / 5 - 10, 40));
    slider.setFillColor(sf::Color(175, 80, 0, 255));
    slider.setPosition(105, top + 145);

    // text
    font.loadFromFile("assets/font.ttf");
    s_algorithms = new string[3];
    s_algorithms[0] = "Dijkstra";
    s_algorithms[1] = "BFS";
    s_algorithms[2] = "A*";

    int offset = 10;

    t1.setFont(font);
    t1.setString(instructions);
    t1.setCharacterSize(20);
    t1.setPosition(offset, top + offset);

    s.setFont(font);
    s.setString(status);
    s.setCharacterSize(20);
    s.setPosition(offset, top + offset + 60);

    h.setFont(font);
    h.setString(heuristic);
    h.setCharacterSize(20);
    h.setPosition(width - 350 - offset, top + offset);

        t_algorithms = new sf::Text[3];
        for (int i = 2; i >= 0; i--) {
            t_algorithms[i].setFont(font);
            t_algorithms[i].setString(s_algorithms[i]);
            t_algorithms[i].setCharacterSize(20);
            // set the them equidistant from each other, using the whole widht
            t_algorithms[i].setPosition(220 + (width - offset * 2) / 3 * i,
                                        top + 150);
        }

        // popup in the middle of screen
        p_bg = sf::RectangleShape(sf::Vector2f(650, 400));
        p_bg.setFillColor(sf::Color(0, 0, 0, 200));
        p_bg.setPosition(width / 2 - 300, (top + height) / 2 - 250);
        p.setFont(font);
        p.setString(popup);
        p.setCharacterSize(22);
        p.setPosition(width / 2 - 265, (top + height) / 2 - 230);
        p.setColor(sf::Color::Red);
    }

  ~HUD() {
    delete[] s_algorithms;
    delete[] t_algorithms;
  }

  void execute() {
    status = "Running";
    s.setString(status);
    matrix->clean();
    // crear grafo
    ListGraph<int, int> graph(matrix);
    Vec2d start = matrix->getStart();
    Vec2d end = matrix->getEnd();
    int *result = nullptr;

    switch (selector) {
    case 0:
      cout << "Ejecutando dijkstra\n";
      Dijkstra(graph, start, end);
      cout << "Terminado\n";
      break;
    case 1:
      cout << "Ejecutando bfs\n";
      GBFS(graph, start, end, h_selector);
      cout << "Terminado\n";
      break;
    case 2:
      cout << "Ejecutando a*\n";
      //A_star(graph, start, end, h_selector);
      A(graph, start, end, h_selector);
      cout << "Terminado\n";
      break;
    }

    status = "Finished";
    s.setString(status);
  }

    void resetStatus() {
        status = "Waiting for input";
        s.setString(status);
    }
    
    void showPopup(bool val) { show_popup = val; }

  void changeSelector(int val) {
    selector = val;
    slider.setPosition(105 + (width - 20) / 3 * val, top + 145);
    if (selector == 0) {
      changeHeuristic(-1);
    } else {
      changeHeuristic(h_selector);
    }
    resetStatus();
  }

  void changeHeuristic(int val) {
    switch (val) {
    case 0:
      h_selector = 0;
      heuristic = "Heuristic: Euclidean";
      break;
    case 1:
      h_selector = 1;
      heuristic = "Heuristic: Manhattan";
      break;
    default:
      heuristic = "";
      break;
    }
    h.setString(heuristic);
    resetStatus();
  }

  void draw() {
    // draw background
    window.draw(bg);

    // draw selector
    window.draw(slider_bg);
    window.draw(slider);

        // draw text
        window.draw(t1);
        window.draw(s);
        window.draw(h);
        for (int i = 2; i >= 0; i--) {
            window.draw(t_algorithms[i]);
        }
        // draw popup
        if (show_popup) {
            window.draw(p_bg);
            window.draw(p);
        }

        window.display();
    }

  // ==================================================
  // ==================== ALGORITMOS ==================
  // ==================================================

  template <typename TV, typename TE>
  void Dijkstra(ListGraph<TV, TE> &Graph, Vec2d &src, Vec2d &treasure) {
    // transformar src y treasure a posiciones
    int end_position = (Graph.get_FullCols() * treasure.y) + treasure.x;
    int src_position = (Graph.get_FullCols() * src.y) + src.x;

    // initial inserts
    int vertices = Graph.get_FullSize();
    int *distances = new int[vertices];
    int *visited = new int[vertices];
    int *parent = new int[vertices];

    // inicializar distancias y visitados
    for (int i = 0; i < vertices; i++) {
      distances[i] = INT_MAX;
      visited[i] = false;
      parent[i] = -1;
    }

    distances[src_position] = 0; // distancia de la fuente a la fuente

    int v{};
    for (int i = 0; i < vertices; i++) {
      v = minDistance(distances, visited,
                      vertices); // vertice con la menor distancia

      if (v == -1) // si no hay mas vertices por visitar
        break;

      visited[v] = true;          // marcar como visitado
      Graph.insertVertex(v);      // insertar vertice en el grafo
      insert_adjacents(v, Graph); // insertar adyacentes del vertice en el grafo Dijkstra

      // pintar vertice visitado
      int x = v % Graph.get_FullCols();
      int y = v / Graph.get_FullCols();
      Vec2d current(x, y);
      if (current != src && current != treasure) {
        Graph.get_matrix()->changeState(x, y, TileType::WalkedOn);
        draw();
        // delay
        sf::sleep(sf::milliseconds(10));
      }

      // actualizar distancias y padres de los adyacentes
      for (auto j : Graph.find(v)) {
        if ((distances[v] + j.second < distances[j.first]) &&
            !visited[j.first]) {
          distances[j.first] = distances[v] + j.second;
          parent[j.first] = v;
        }
      }

      if (v == end_position) { // verificar si llego al tesoro
        break;
      }
    }

    if (v == end_position) {
      //  ============ DRAW CAMINO ============
      v = parent[v];
      while (v != src_position) {
        int x = v % Graph.get_FullCols();
        int y = v / Graph.get_FullCols();
        Graph.get_matrix()->changeState(x, y, TileType::Path);
        draw();
        v = parent[v];
      }
      cout << "<<< Destino encontrado >>>" << endl;
    } else {
      cout << "<<< Destino NO encontrado >>>" << endl;
    }

    delete[] distances;
    delete[] visited;
    delete[] parent;
  }

  template <typename TV, typename TE>
  void GBFS(ListGraph<TV, TE> &Graph, Vec2d &src, Vec2d &treasure,
            int heuristic) {
    // transformar src y treasure a posiciones
    int end_position = (Graph.get_FullCols() * treasure.y) + treasure.x;
    int src_position = (Graph.get_FullCols() * src.y) + src.x;

    // initial inserts
    int vertices = Graph.get_FullSize();
    int *distances = new int[vertices];
    int *visited = new int[vertices];
    int *parent = new int[vertices];

    // inicializar distancias y visitados
    for (int i = 0; i < vertices; i++) {
      distances[i] = INT_MAX;
      visited[i] = false;
      parent[i] = -1;
    }

    Heap<pair<Vec2d, double>> pq;
    pq.heap_push(make_pair(src, 0));

    int current_pos;
    while (pq.get_size() != 0) {
      auto current = pq.heap_pop();
      current_pos = (Graph.get_FullCols() * current.first.y) + current.first.x;

      // check si llegamos a treasure
      if (current.first == treasure) {
        cout << "Goal node reached!" << endl;
        break;
      }

      visited[current_pos] = true;
      // BFS insert adyacentes
      insert_adjacents(current_pos, Graph, pq, treasure, heuristic);

      // pintar vertice visitado
      int x = current_pos % Graph.get_FullCols();
      int y = current_pos / Graph.get_FullCols();
      Vec2d curr(x, y);
      if (curr != src && curr != treasure) {
        Graph.get_matrix()->changeState(x, y, TileType::WalkedOn);
        draw();
        // delay
        sf::sleep(sf::milliseconds(10));
      }
      // actualizar distancias y padres de los adyacentes
      for (auto j : Graph.find(current_pos)) {
        if ((distances[current_pos] + j.second < distances[j.first]) &&
            !visited[j.first]) {
          distances[j.first] = distances[current_pos] + j.second;
          parent[j.first] = current_pos;
        }
      }
    }
    if (current_pos == end_position) {
      //  ============ DRAW CAMINO ============
      current_pos = parent[current_pos];
      while (current_pos != src_position) {
        int x = current_pos % Graph.get_FullCols();
        int y = current_pos / Graph.get_FullCols();
        Graph.get_matrix()->changeState(x, y, TileType::Path);
        draw();
        current_pos = parent[current_pos];
      }
      cout << "<<< Destino encontrado >>>" << endl;
    } else {
      cout << "<<< Destino NO encontrado >>>" << endl;
    }

    delete[] distances;
    delete[] visited;
    delete[] parent;
  }

  // ------------------------------ A* ------------------------------
  template <typename TV, typename TE>
  void A(ListGraph<TV, TE> &Graph, Vec2d &src, Vec2d &treasure,
            int heuristic) {
    // transformar src y treasure a posiciones
    int end_position = (Graph.get_FullCols() * treasure.y) + treasure.x;
    int src_position = (Graph.get_FullCols() * src.y) + src.x;

    // initial inserts
    int vertices = Graph.get_FullSize();
    int *distances = new int[vertices];
    int *visited = new int[vertices];
    int *parent = new int[vertices];

    // inicializar distancias y visitados
    for (int i = 0; i < vertices; i++) {
      distances[i] = INT_MAX;
      visited[i] = false;
      parent[i] = -1;
    }

    Heap<pair<Vec2d, double>> pq;
    pq.heap_push(make_pair(src, 0));
    distances[src_position] = 0;

    int current_pos;
    while (pq.get_size() != 0) {
      auto current = pq.heap_pop();
      current_pos = (Graph.get_FullCols() * current.first.y) + current.first.x;

      // check si llegamos a treasure
      if (current.first == treasure) {
        cout << "Goal node reached!" << endl;
        break;
      }

      visited[current_pos] = true;
        // A* insert
      insert_adjacents(current_pos, Graph, pq, treasure, heuristic, distances);

      // pintar vertice visitado
      int x = current_pos % Graph.get_FullCols();
      int y = current_pos / Graph.get_FullCols();
      Vec2d curr(x, y);
      if (curr != src && curr != treasure) {
        Graph.get_matrix()->changeState(x, y, TileType::WalkedOn);
        draw();
        // delay
        sf::sleep(sf::milliseconds(10));
      }
      // actualizar distancias y padres de los adyacentes
      for (auto j : Graph.find(current_pos)) {
        if ((distances[current_pos] + j.second < distances[j.first]) &&
            !visited[j.first]) {
          distances[j.first] = distances[current_pos] + j.second;
          parent[j.first] = current_pos;
        }
      }
    }
    if (current_pos == end_position) {
      //  ============ DRAW CAMINO ============
      current_pos = parent[current_pos];
      while (current_pos != src_position) {
        int x = current_pos % Graph.get_FullCols();
        int y = current_pos / Graph.get_FullCols();
        Graph.get_matrix()->changeState(x, y, TileType::Path);
        draw();
        current_pos = parent[current_pos];
      }
      cout << "<<< Destino encontrado >>>" << endl;
    } else {
      cout << "<<< Destino NO encontrado >>>" << endl;
    }

    delete[] distances;
    delete[] visited;
    delete[] parent;
  }
};