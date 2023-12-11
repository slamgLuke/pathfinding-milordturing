#pragma once

#include "graph.h"
#include "heap.h"
#include "limits.h"
#include <cmath>

// Funcion que retorna el index del vertice con la menor distancia
template <typename T>
int minDistance(T distances[], int visited[], int Vertices) {
  T min_distance = INT_MAX;
  int min_index;

  for (int v = 0; v < Vertices; v++) {
    if (visited[v] == false && distances[v] <= min_distance) {
      min_distance = distances[v];
      min_index = v;
    }
  }
  if (min_distance !=
      INT_MAX) { // retorna el index del vertice con la menor distancia
    return min_index;
  } else { // retorna -1 si no hay mas vertices por visitar
    return -1;
  }
}

double get_heuristic(Vec2d &v1, Vec2d &v2, int heuristic) {
    // Escala para el peso de las aristas:
    // ->   10 misma escala que los pesos de las aristas
    // ->   Si se cambia la escala Puede predomiar el peso de las aristas o la heuristica en el A_star
    double escala = 10;  

  // = 0, euclidian
  if (heuristic == 0)
    return (std::sqrt(std::pow(int(v2.x) - int(v1.x), 2) +
                     std::pow(int(v2.y) - int(v1.y), 2)))*escala;
  // = 1, manhattan
  else
    return (std::abs(int(v2.x) - int(v1.x)) + std::abs(int(v2.y) - int(v1.y)))*escala;
}

// ---- Insertar los adyacentes de un nodo en el grafo y crea las aristas con sus pesos ----

// ========================= Dijkstra =========================
template <typename TV, typename TE>
void insert_adjacents(int pos, ListGraph<TV, TE> &Graph) {
  int col = (pos) % Graph.get_FullCols();
  int row = (pos - col) / Graph.get_FullCols();
  int V1 = (Graph.get_FullCols() * row) + col;

  // Recorre los nodos adyacentes
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int new_col = col + i;
      int new_row = row + j;

      // Verifica que las coordenadas estén dentro de los límites del grafo
      // (matriz)
      if (new_col >= 0 && new_col < Graph.get_FullCols() && new_row >= 0 &&
          new_row < Graph.get_FullRows() &&
          Graph.getTile(new_col, new_row)->type != Wall) {

        // Calcula el index del vertice adyacente
        int V2 = (Graph.get_FullCols() * new_row) + new_col;

        // Crea la arista con su peso e inserta el vertice
        if ((i == 0 || j == 0) && i != j) { // Adyacente en linea recta
            if (!Graph.exists(V2))
                Graph.insertVertex(V2);
          
          Graph.createEdge(V1, V2, 10);
        } 
        else if (i != 0 && j != 0) { // Adyacente en diagonal
            if (!Graph.exists(V2))
                Graph.insertVertex(V2);
            
            Graph.createEdge(V1, V2, 14);
        }
      }
    }
  }
}

// ========================= BFS =========================
template <typename TV, typename TE>
void insert_adjacents(int pos, ListGraph<TV, TE> &Graph,
                      Heap<pair<Vec2d, double>> &pq, Vec2d &treasure,
                      int heuristic) {
  int col = (pos) % Graph.get_FullCols();
  int row = (pos - col) / Graph.get_FullCols();
  int V1 = (Graph.get_FullCols() * row) + col;

  // Recorre los nodos adyacentes
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int new_col = col + i;
      int new_row = row + j;

      // Verifica que las coordenadas estén dentro de los límites del grafo
      // (matriz)
      if (new_col >= 0 && new_col < Graph.get_FullCols() && new_row >= 0 &&
          new_row < Graph.get_FullRows() &&
          Graph.getTile(new_col, new_row)->type != Wall) {

        // Calcula el index del vertice adyacente
        int V2 = (Graph.get_FullCols() * new_row) + new_col;

        // Crea la arista con su peso e inserta el vertice
          if ((i == 0 || j == 0) && i != j) { // Adyacente en linea recta
            if (!Graph.exists(V2)) {
            Vec2d temp(new_col, new_row);
            pq.heap_push(
                make_pair(temp, get_heuristic(temp, treasure, heuristic)));
                Graph.insertVertex(V2);
            }
            
            Graph.createEdge(V1, V2, 10);
          } else if (i != 0 && j != 0) { // Adyacente en diagonal
            if (!Graph.exists(V2)) {
            Vec2d temp(new_col, new_row);
            pq.heap_push(
                make_pair(temp, get_heuristic(temp, treasure, heuristic)));
                Graph.insertVertex(V2);
            }
            Graph.createEdge(V1, V2, 14);
          }
        
      }
    }
  }
}

// ============================= A* =============================
template <typename TV, typename TE>
void insert_adjacents(int pos, ListGraph<TV, TE> &Graph,
                      Heap<pair<Vec2d, double>> &pq, Vec2d &treasure,
                      int heuristic, int *distances) {
  int col = (pos) % Graph.get_FullCols();
  int row = (pos - col) / Graph.get_FullCols();
  int V1 = (Graph.get_FullCols() * row) + col;

  // Recorre los nodos adyacentes
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int new_col = col + i;
      int new_row = row + j;

      // Verifica que las coordenadas estén dentro de los límites del grafo
      // (matriz)
      if (new_col >= 0 && new_col < Graph.get_FullCols() && new_row >= 0 &&
          new_row < Graph.get_FullRows() &&
          Graph.getTile(new_col, new_row)->type != Wall) {

        // Calcula el index del vertice adyacente
        int V2 = (Graph.get_FullCols() * new_row) + new_col;

        // Crea la arista con su peso e inserta el vertice        
          if ((i == 0 || j == 0) && i != j) { // Adyacente en linea recta
            if (!Graph.exists(V2)) {
            Vec2d temp(new_col, new_row);
            // Para el A* se suma la distancia del nodo actual + 10 para su adyacente
            pq.heap_push(make_pair(temp, get_heuristic(temp, treasure, heuristic) + distances[V1]+10));
                Graph.insertVertex(V2);
            }
            
            Graph.createEdge(V1, V2, 10);
          } else if (i != 0 && j != 0) { // Adyacente en diagonal
            if (!Graph.exists(V2)) {
            Vec2d temp(new_col, new_row);
            // Para el A* se suma la distancia del nodo actual + 14 para su adyacente
            pq.heap_push(make_pair(temp, get_heuristic(temp, treasure, heuristic) + distances[V1]+14));  
            Graph.insertVertex(V2);
            }
            Graph.createEdge(V1, V2, 14);
          }
        
      }
    }
  }
}
