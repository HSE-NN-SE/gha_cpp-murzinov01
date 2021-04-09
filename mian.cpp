#include <cstddef>
#include <cassert>
#include <initializer_list>
#include <iostream>


template <typename T>
class vector {
public:
  vector() : my_capacity(0), my_size(0), my_data(nullptr) {}
  vector(const vector& vec) : my_capacity(vec.my_capacity), my_size(vec.my_size)
    , my_data(static_cast<T*>(::operator new(sizeof(T)* my_capacity)))
  {
    for (std::size_t i = 0; i < my_size; ++i) {
      new(my_data + i) T(vec.my_data[i]);
    }
  }

  // homework
  vector(vector&& vec) noexcept : my_capacity(vec.my_capacity), my_size(vec.my_size)
    , my_data(vec.my_data)
  {
    vec.my_data = nullptr;
    vec.my_capacity = 0;
    vec.my_size = 0;
  }

  vector(std::size_t size, const T& v)
    : my_capacity(size)
    , my_size(size)
    , my_data(static_cast<T*>(::operator new(sizeof(T)* my_capacity)))
  {
    for (int i = 0; i < my_size; i++) {
      new (my_data + i) T(v);
    }
  }

  vector(std::initializer_list<T> init_list) :
    my_capacity(init_list.size()),
    my_size(init_list.size()),
    my_data(new T[my_capacity])
  {
    std::size_t i{ 0 };
    for (auto& value : init_list) {
      my_data[i] = value;
      ++i;
    }
  }

  ~vector() {
    clear();
  }

  vector& operator=(const vector& vec) {
    if (this != &vec) {
      delete[] my_data;
      my_capacity = vec.my_capacity;
      my_size = vec.my_size;
      my_data = new T[my_capacity];
      for (std::size_t i = 0; i < my_size; ++i) {
        my_data[i] = vec.my_data[i];
      }
    }
    return *this;
  }

  // homework
  vector& operator=(vector&& vec) noexcept {
    if (this != &vec) {
      delete[] my_data;
      my_capacity = vec.my_capacity;
      my_size = vec.my_size;
      my_data = vec.my_data;
      vec.my_capacity = 0;
      vec.my_size = 0;
      vec.my_data = nullptr;
    }
    return *this;
  }

  T& operator[](std::size_t index) {
    assert(index < my_size);
    return my_data[index];
  }

  const T& operator[](std::size_t index) const {
    assert(index < my_size);
    return my_data[index];
  }

  void clear() {
    for (int i = 0; i < my_size; i++) {
      my_data[i].~T();
    }
    operator delete(my_data);
    my_data = nullptr;
    my_capacity = 0;
    my_size = 0;
  }

  void push_back(const T& v) {
    if (my_capacity == my_size) {
      if (my_capacity == 0) {
        my_capacity = 1;
      }
      else {
        my_capacity *= 2;
      }

      T* tmp = new T[my_capacity];

      for (std::size_t i = 0; i < my_size; ++i) {
        tmp[i] = my_data[i];
      }

      delete[] my_data;
      my_data = tmp;
    }

    my_data[my_size] = v;
    ++my_size;
  }

  void pop_back() {
    if (my_size > 0) {
      --my_size;
      my_data[my_size].~T();
    }
  }

  void resize(std::size_t size, const T& val = T()) {
    if (size < my_size) {
      for (std::size_t i = size; i < my_size; ++i) {
        my_data[i].~T();
      }
    }
    else if (size > my_size) {
      if (size > my_capacity) {
        my_capacity = size;
        T* tmp = static_cast<T*>(::operator new(sizeof(T) * my_capacity));

        for (std::size_t i = 0; i < my_size; ++i) {
          new(tmp + i) T(std::move(my_data[i]));
          my_data[i].~T();
        }

        operator delete(my_data);
        my_data = tmp;
      }
      for (std::size_t i = my_size; i < size; ++i) {
        new(my_data + i) T(val);
      }
    }
    my_size = size;
  }

  void reserve(std::size_t new_capacity) {
    if (new_capacity > my_capacity) {
      my_capacity = new_capacity;
      T* tmp = new T[new_capacity];
      for (std::size_t i = 0; i < my_size; ++i) {
        tmp[i] = my_data[i];
      }
      delete[] my_data;
      my_data = tmp;
    }
  } // homework

  std::size_t size() const {
    return my_size;
  }

private:
  std::size_t my_capacity;
  std::size_t my_size;

  T* my_data;
};


int main() {
  vector<int> vec_def(7, 3);
  for (int i = 0; i < vec_def.size(); i++) {
    std::cout << vec_def[i] << " ";
  }
  std::cout << std::endl;
  vector<int> vec{ 1, 2, 3, 4, 5, 6, 7 };
  for (int i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
  vector<int> vec_copy{ vec };
  for (int i = 0; i < vec_copy.size(); i++) {
    std::cout << vec_copy[i] << " ";
  }
  std::cout << std::endl;
  vec.pop_back();
  for (int i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
  vector<int> vec1 = std::move(vec);

  for (int i = 0; i < vec1.size(); i++) {
    std::cout << vec1[i] << " ";
  }
  std::cout << std::endl;

  vector<int> vec2{ 1, 2, 3, 4 };
  vec1 = std::move(vec2);
  for (int i = 0; i < vec1.size(); i++) {
    std::cout << vec1[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Resize" << std::endl;
  vector<int> vec_resize{ 1, 2, 3, 4, 5, 6, 7 };
  vec_resize.resize(3);
  for (int i = 0; i < vec_resize.size(); i++) {
    std::cout << vec_resize[i] << " ";
  }
  std::cout << std::endl;
  vec_resize.resize(6, 3);
  for (int i = 0; i < vec_resize.size(); i++) {
    std::cout << vec_resize[i] << " ";
  }
  std::cout << std::endl;
  vec_resize.resize(12, 3);
  for (int i = 0; i < vec_resize.size(); i++) {
    std::cout << vec_resize[i] << " ";
  }
  std::cout << std::endl;
}
