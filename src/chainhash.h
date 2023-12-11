#include <forward_list>
#include <iostream>
#include <vector>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.5;

template <typename TK, typename TV> class Hash {
private:
  struct Entry {
    TK key;
    TV value;
    size_t hashcode;
  };
  forward_list<Entry> *array; // array de listas enlazadas
  int capacity;               // tamanio del array
  int size;                   // cantidad de elementos totales

public:
  Hash(int _cap = 10) {
    // TODO: asignar un tamanio inicial al array
    capacity = _cap;
    array = new forward_list<Entry>[capacity];
    size = 0;
  }

  ~Hash() { delete[] array; }

  void insert(TK key, TV value) {
    if (fillFactor() >= maxFillFactor)
      rehashing();

    size_t hashcode = getHashCode(key);
    int index = hashcode % capacity;
    // TODO: insertar el Entry(key, value) en array[index] verificando si la
    // key ya existe
    int num_colision = 0;
    for (auto it = array[index].begin(); it != array[index].end(); it++) {
      if (it->key == key) {
        it->value = value;
        return;
      }
      ++num_colision;
    }

    array[index].push_front(Entry{key, value, hashcode});
    ++size;

    if (num_colision == maxColision)
      rehashing();
  }

  TV &find(TK key) {
    size_t hashcode = getHashCode(key);
    int index = hashcode % capacity;

    for (auto it = array[index].begin(); it != array[index].end(); it++) {
      if (it->key == key) {
        return it->value;
      }
    }
    throw "Key not found";
  }

  bool exists(TK key) {
    size_t hashcode = getHashCode(key);
    int index = hashcode % capacity;

    for (auto it = array[index].begin(); it != array[index].end(); it++) {
      if (it->key == key) {
        return true;
      }
    }
    return false;
  }

  void remove(TK key) {
    size_t hashcode = getHashCode(key);
    int index = hashcode % capacity;

    for (auto it = array[index].begin(); it != array[index].end(); it++) {
      if (it->key == key) {
        array[index].erase(it);
        --size;
        return;
      }
    }
  }

  // TODO: implementar el operador corchete [ ]
  int bucket_count() { return capacity; }
  int bucket_size(int n) {
    int count = 0;
    for (auto it = array[n].begin(); it != array[n].end(); it++) {
      ++count;
    }
    return count;
  }

  // operador []
  TV &operator[](TK key) {
    size_t hashcode = getHashCode(key);
    int index = hashcode % capacity;
    int num_colision = 0;

    for (auto it = array[index].begin(); it != array[index].end(); it++) {
      if (it->key == key) {
        return it->value;
      }
      ++num_colision;
    }
    array[index].push_front(Entry{key, TV(), hashcode});
    ++size;
    if (num_colision == maxColision)
      rehashing();

    return find(key);
  }

  int getSize() { return size; }

  // begin(i) return forward_list<Entry>::iterator de la lista en el bucket i
  // end(i) return forward_list<Entry>::iterator de la lista en el bucket i
  typename forward_list<Entry>::iterator begin(int i) {
    return array[i].begin();
  }
  typename forward_list<Entry>::iterator end(int i) { return array[i].end(); }

  vector<pair<TK, TV>> get_all() {
    vector<pair<TK, TV>> v;
    for (int i = 0; i < capacity; i++) {
      for (auto it = array[i].begin(); it != array[i].end();
           it++) { // constante
        v.push_back(make_pair(it->key, it->value));
      }
    }
    return v;
  }

  void clear() {
    delete[] array;
    capacity = 10;
    array = new forward_list<Entry>[capacity];
    size = 0;
  }

private:
  double fillFactor() { return (float)size / (capacity * maxColision); }

  size_t getHashCode(TK key) {
    std::hash<TK> ptr_hash;
    return ptr_hash(key);
  }

  void rehashing() {
    int new_capacity = capacity * 2;
    forward_list<Entry> *new_array = new forward_list<Entry>[new_capacity];
    for (int i = 0; i < capacity; i++) {
      for (auto it = array[i].begin(); it != array[i].end(); it++) {
        size_t hashcode = getHashCode(it->key);
        int index = hashcode % new_capacity;
        new_array[index].push_front(Entry{it->key, it->value, hashcode});
      }
    }
    delete[] array;
    array = new_array;
    capacity = new_capacity;
  }
};
