# C++ Hash Module

This C++ module provides a hash table implementation for storing sequences of uint64_t numbers. It consists of a header file, `hash.h`, which declares the module's interface, and an implementation file, `hash.cc`, which contains its implementation. This module offers the following functions:

```cpp
unsigned long hash_create(hash_function_t hash_function);
```

Creates a hash table and returns its identifier. The `hash_function` parameter is a pointer to a hash function that returns a uint64_t number. The hash function takes parameters of type `uint64_t const *` and `size_t` in that order.

```cpp
void hash_delete(unsigned long id);
```

Deletes the hash table with the given `id` if it exists. Otherwise, it does nothing.

```cpp
size_t hash_size(unsigned long id);
```

Returns the number of sequences stored in the hash table with the given `id`, or 0 if the table doesn't exist.

```cpp
bool hash_insert(unsigned long id, uint64_t const * seq, size_t size);
```

Inserts a sequence of `size` integers (`seq`) into the hash table with the given `id`. Returns true if the operation succeeds. The operation fails if the hash table doesn't exist, the table already contains the sequence, `seq` parameter is NULL, or `size` parameter is 0.

```cpp
bool hash_remove(unsigned long id, uint64_t const * seq, size_t size);
```

Removes the sequence of `size` integers (`seq`) from the hash table with the given `id`. Returns true if the operation succeeds. The operation fails if the hash table doesn't exist, the table doesn't contain the sequence, `seq` parameter is NULL, or `size` parameter is 0.

```cpp
void hash_clear(unsigned long id);
```

If the hash table with the given `id` exists and is not empty, removes all elements from it. Otherwise, it does nothing.

```cpp
bool hash_test(unsigned long id, uint64_t const * seq, size_t size);
```

Returns true if the hash table with the given `id` exists and contains the sequence of `size` integers (`seq`). Returns false otherwise or if the `seq` parameter is NULL or `size` parameter is 0.

All global variables and helper functions that are not part of the module's interface should be hidden from the external world.

The functions should output diagnostic information to the standard error stream. Parameter validation and function execution can be verified using assertions. Compiling with the `-DNDEBUG` flag should disable the output and assertions. The handling of the standard error output should be implemented using C++ streams.

A NULL value for the hash function pointer is considered invalid.

The expected solution should make use of containers and methods provided by the standard C++ library. Custom structures or classes should not be defined, except possibly for creating a hash object. Pointers to `uint64_t const *` passed by the user should not be stored directly since the user may modify the data or deallocate the memory after the operation.

Excessive use of conditional compilation should be avoided in the solution. The source code fragments that implement the specified hash module operations should not depend on the method of compilation (-DNDEBUG parameter or its absence), as diagnostic information output wouldn't make sense otherwise.
