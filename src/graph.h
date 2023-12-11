#include "chainhash.h"
#include "matrix.h"
#include <list>
using namespace std;

template <typename TV, typename TE> struct Edge {
  TV init;
  TV end;
  TE weight;
  Edge(TV i, TV e, TE w = 0) {
    init = i;
    end = e;
    weight = w;
  }
  // comparador segun peso
  bool operator<(const Edge &e) const { return weight < e.weight; }
  bool operator>(const Edge &e) const { return weight > e.weight; }
};

template <typename TV, typename TE> class ListGraph {
private:
  Hash<TV, list<pair<TV, TE>>> graph;
  vector<TV> vertices;
  Matrix *matrix;

public:
  ListGraph(Matrix *matrix) : matrix(matrix) {}
  ~ListGraph() {}

  void insertVertex(const TV &vertex) {
    graph.insert(vertex, list<pair<TV, TE>>{});
    vertices.push_back(vertex);
  }

  void createEdge(TV &vertex1, TV &vertex2, TE weight) {
    for (auto it = graph[vertex1].begin(); it != graph[vertex1].end(); it++) {
      if (it->first == vertex2) {
        it->second = weight;
        return;
      }
    }
    graph[vertex1].push_back(make_pair(vertex2, weight));
  }

  int get_FullCols() { return matrix->getCols(); }
  int get_FullRows() { return matrix->getRows(); }
  int get_FullSize() { return matrix->getCols() * matrix->getRows(); }
  Tile *getTile(int x, int y) { return matrix->getTile(x, y); }

  vector<TV> &getVertices() { return vertices; }

  Matrix *get_matrix() { return matrix; }

  void clear() { graph.clear(); }

  list<pair<TV, TE>> find(TV element) { return graph.find(element); }

  bool exists(TV element) { return graph.exists(element); }
};
