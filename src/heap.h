// Min-heap with array
#include <initializer_list>
#include <iostream>

using namespace std;

template <typename T>
// Sobrecarga del operador < para std::pair<Vec2d, float>
bool operator<(const std::pair<T, double> &lhs,
               const std::pair<T, double> &rhs) {
  return lhs.second < rhs.second;
}

template <typename T>
// Sobrecarga del operador > para std::pair<Vec2d, float>
bool operator>(const std::pair<T, double> &lhs,
               const std::pair<T, double> &rhs) {
  return lhs.second > rhs.second;
}

template <class T> class Heap {

private:
  T *arr;
  int size, capacity;

  void heapify_up(int idx) {
    if (idx <= 0 || idx >= size)
      return;

    int parent = (idx - 1) / 2;
    if (arr[idx] < arr[parent]) {
      swap(arr[idx], arr[parent]);
      heapify_up(parent);
    }
  }

  void heapify_down(int idx) {
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    int smallest = idx;

    if (left < size && arr[left] < arr[smallest]) {
      smallest = left;
    }
    if (right < size && arr[right] < arr[smallest]) {
      smallest = right;
    }

    if (smallest != idx) {
      swap(arr[idx], arr[smallest]);
      heapify_down(smallest);
    }
  }

  void resize() {
    T *temp = new T[capacity * 2];
    for (int i = 0; i < size; ++i) {
      temp[i] = arr[i];
    }
    delete[] arr;
    arr = temp;
    this->capacity *= 2;
  }

public:
  Heap(int _capacity = 10) {
    capacity = _capacity;
    size = 0;
    arr = new T[capacity];
  }

  Heap(initializer_list<T> list) {
    this->capacity = list.size() * 2;
    this->size = list.size();
    arr = new T[capacity + 1];
    for (int i = 0; i < size; ++i) {
      arr[i] = *(list.begin() + i);
    }
  }

  ~Heap() { delete[] arr; }

  T &operator[](int idx) {
    if (idx < 0 || idx >= size)
      throw "Index out of range";
    return arr[idx];
  }

  int get_size() { return size; }
  int get_capacity() { return capacity; }

  void heap_push(T val) {
    if (size == capacity)
      resize();
    arr[size] = val;
    ++size;

    int idx = size - 1;
    heapify_up(idx);
  }

  T heap_pop() {
    if (size == 0)
      throw "Heap vacio";
    T val = arr[0];
    arr[0] = arr[size - 1];
    --size;
    heapify_down(0);

    return val;
  }

  void heap_sort() {
    int temp_size = size;
    for (int i = 0; i < temp_size; ++i) {
      T val = heap_pop();
      arr[temp_size - 1 - i] = val;
    }
    size = temp_size;
  }

  void display() {
    for (int i = 0; i < size; ++i) {
      cout << arr[i] << " ";
    }
    cout << endl;
  }

  friend void make_heap(Heap<T> &a) {
    int last_parent = (a.size - 2) / 2;
    for (int node = last_parent; node >= 0; --node) {
      a.heapify_down(node);
    }
  }
};

template <class T> void heap_push(Heap<T> &arr, T val) { arr.heap_push(val); }

template <class T> T heap_pop(Heap<T> &arr) { return arr.heap_pop(); }

template <class T> void heap_sort(Heap<T> &arr) { arr.heap_sort(); }
