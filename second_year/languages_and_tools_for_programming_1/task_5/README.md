# kvfifo Container

The goal of this task is to implement a container pattern that behaves like a FIFO queue, where each element has an associated key. This container should provide strong exception safety guarantees and implement copy-on-write semantics.

Copy-on-write is an optimization technique widely used, among others, in data structures in the Qt library and previously in implementations of std::string. The basic idea is that when we create a copy of an object (in C++ using the copy constructor or assignment operator), it shares all internal resources (which can be stored in a separate object on the heap) with the source object. This state persists until one of the copies needs to be modified. At that point, the modifying object creates its own copy of the resources and performs the modifications on them.

The pattern should be parameterized by the key and value types, denoted by K and V, respectively. The key type K has value semantics, meaning that it has a default constructor, copy constructor, move constructor, and assignment operators available. The K type has a linear order defined, and all comparison operations can be performed on objects of this type. For the value type V, only the copy constructor is assumed to be available.

As part of this task, you should implement the following template:

```cpp
template <typename K, typename V> class kvfifo;
```

The `kvfifo` class should provide the operations described below. Each operation includes its expected time complexity assuming no copies need to be made. The expected time complexity of copy-on-write operations is O(n log n), where n is the number of elements stored in the queue. All operations must provide at least strong exception safety guarantees, and the move constructor and destructor must be no-throw.

- Constructors: default constructor creating an empty queue, copy constructor, and move constructor. Complexity: O(1).

  ```cpp
  kvfifo();
  kvfifo(kvfifo const &);
  kvfifo(kvfifo &&);
  ```

- Assignment operator taking the argument by value. Complexity: O(1) plus the time to destroy the overwritten object.

  ```cpp
  kvfifo& operator=(kvfifo other);
  ```

- The `push` method inserts the value `v` at the end of the queue with the key `k`. Complexity: O(log n).

  ```cpp
  void push(K const &k, V const &v);
  ```

- The `pop` method removes the first element from the queue. If the queue is empty, it throws an `std::invalid_argument` exception. Complexity: O(log n).

  ```cpp
  void pop();
  ```

- The `pop(k)` method removes the first element with the given key from the queue. If the specified key is not in the queue, it throws an `std::invalid_argument` exception. Complexity: O(log n).

  ```cpp
  void pop(K const &);
  ```

- The `move_to_back` method moves the elements with key `k` to the back of the queue, preserving their relative order. It throws an `std::invalid_argument` exception if there is no element with the given key in the queue. Complexity: O(m + log n), where m is the number of moved elements.

  ```cpp
  void move_to_back(K const &k);
  ```

- The `front` and `back` methods return a pair of references to the key and value at the beginning and end of the queue, respectively. In the non-const version, the returned pair should allow modifying the value but not the key. Any operation that modifies the queue may invalidate the returned references. If the queue is empty, it throws an `std::invalid_argument` exception. Complexity: O(1).

  ```cpp
  std::pair<K const &, V &> front();
  std::pair<K const &, V const &> front() const;
  std::pair<K const &, V &> back();
  std::pair<K const &, V const &> back() const;
  ```

- The `first` and `last` methods return the first and last key-value pair with the given key, similar to `front` and `back`. If the specified key is not in the queue, it throws an `std::invalid_argument` exception. Complexity: O(log n).

  ```cpp
  std::pair<K const &, V &> first(K const &key);
  std::pair<K const &, V const &> first(K const &key) const;
  std::pair<K const &, V &> last(K const &key);
  std::pair<K const &, V const &> last(K const &key) const;
  ```

- The `size` method returns the number of elements in the queue. Complexity: O(1).

  ```cpp
  size_t size() const;
  ```

- The `empty` method returns true if the queue is empty, and false otherwise. Complexity: O(1).

  ```cpp
  bool empty() const;
  ```

- The `count` method returns the number of elements in the queue with the given key. Complexity: O(log n).

  ```cpp
  size_t count(K const &) const;
  ```

- The `clear` method removes all elements from the queue. Complexity: O(n).

  ```cpp
  void clear();
  ```

- The `k_iterator` iterator and the `k_begin` and `k_end` methods allow iterating over the set of keys in ascending order of their values. Iterators may be invalidated by any modifying operation that completes successfully, as well as by the `front`, `back`, `first`, and `last` operations in their non-const versions. The iterator must satisfy the `std::bidirectional_iterator` concept. All operations have a time complexity of O(log n). Iterating over the entire queue has a time complexity of O(n). The iterator is used only for iterating over the queue and cannot be used to modify the queue, behaving like a const_iterator in the standard library.

Where possible and appropriate, methods should be qualified with `const` and `noexcept`.

The `kvfifo` class should be exception-transparent, meaning that it should allow any exceptions raised by the functions it calls and operations on its members to propagate through, and the observable state of objects should not change. In particular, unsuccessful modifying operations should not invalidate iterators.