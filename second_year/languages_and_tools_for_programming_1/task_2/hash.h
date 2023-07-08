#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus   // C++ headers
#include <iostream>  //cerr initialisation
#else                // C headers
#include <stdbool.h> //bool
#include <stdint.h>  //uint64_t
#include <stdio.h>   //cerr initialisation
#endif

#ifdef __cplusplus
namespace jnp1 {
extern "C" {
#endif

// Must use 'typedef' instead of 'using' so it compiles in C
typedef uint64_t (*hash_function_t)(uint64_t const *, size_t);

extern unsigned long hash_create(hash_function_t hash_function);
extern void hash_delete(unsigned long id);
extern size_t hash_size(unsigned long id);
extern bool hash_insert(unsigned long id, uint64_t const *seq, size_t size);
extern bool hash_remove(unsigned long id, uint64_t const *seq, size_t size);
extern void hash_clear(unsigned long id);
extern bool hash_test(unsigned long id, uint64_t const *seq, size_t size);

#ifdef __cplusplus
} // extern C
} // namespace jnp1
#endif

#endif /*HASH_H*/
