#include "hash.h"
#include <assert.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef NDEBUG
bool constexpr debug = false;
#else
bool constexpr debug = true;
#endif

namespace jnp1 {

//+++++ HELPFUL STRUCTURES ++++

using hash_function_t = uint64_t (*)(uint64_t const *, size_t);

// Hash object, keeper of the hash_function
class Hasher {
  private:
    hash_function_t proto_hasher;

  public:
    uint64_t operator()(std::vector<uint64_t> vector) const {
        return proto_hasher(&vector[0], vector.size());
    }
    Hasher(hash_function_t function) { proto_hasher = function; }
};

using hash_table_t = std::unordered_set<std::vector<uint64_t>, Hasher>;

// Type of the map that keeps the hashes: there is only one
using hash_map = std::unordered_map<unsigned long, hash_table_t *>;

// Constructed on first use to avoid static order initialisation fiasco
hash_map &map_of_hashes() {
    static hash_map *map_of_hashes = new hash_map;
    return *map_of_hashes;
}

//+++++ HELPFUL FUNCTIONS FOR DEBUGGING +++++
// Functions start_debug and helpers have two purposes:
// 1. They check whether the arguments provided are correct: if so, they return
// true, otherwise false
// 2. If debug is true, they print appropriate diagnostic information

// Write seq to string
static std::string seq_to_string(uint64_t const *seq, const size_t size) {
    if (seq == nullptr)
        return "NULL";

    std::string seq_string("\"");
    if (size > 0)
        seq_string += std::to_string(seq[0]);
    for (size_t i = 1; i < size; i++)
        seq_string += " " + std::to_string(seq[i]);
    seq_string += "\"";

    return seq_string;
}

// Checks whether seq is null or size is 0: if so, returns true, otherwise false
static bool is_null_or_zero(const std::string &function_name,
                            uint64_t const *seq, const size_t size) {
    bool is_wrong = (seq == nullptr || size == 0);
    if (debug && seq == nullptr) {
        std::cerr << function_name << ": "
                  << "invalid pointer (NULL)\n";
    }
    if (debug && size == 0) {
        std::cerr << function_name << ": "
                  << "invalid size (0)\n";
    }
    return is_wrong;
}

// Checks whether hash function with a given id is in map_of_hashes
static bool is_hash_present(const std::string &function_name, const size_t id) {

    if (map_of_hashes().find(id) == map_of_hashes().end()) {
        if (debug)
            std::cerr << function_name << ": hash table #" << std::to_string(id)
                      << " does not exist\n";
        return false;
    }
    return true;
}

// Print debug message at the beginning (if debug is on), checks whether input
// is correct, (functions with 3 args)
//
bool start_debug_3_args(const std::string &function_name, unsigned long id,
                        uint64_t const *seq, size_t size) {
    if (debug)
        std::cerr << function_name << "(" << id << ", "
                  << seq_to_string(seq, size) << ", " << size << ")\n";

    bool is_correct = !is_null_or_zero(function_name, seq, size) &&
                      is_hash_present(function_name, id);

    return is_correct;
}

// Version for hash_function_t argument
static void start_debug_1_arg(const std::string &function_name,
                              const void *arg) {
    if (debug) {
        std::cerr << function_name << "(" << arg << ")\n";
        assert(arg != nullptr);
    }
}

// Prints final debug information if debug is true
static void end_debug_message(const std::string &function_name,
                              const unsigned long id,
                              const std::string &debug_message) {
    if (debug)
        std::cerr << function_name << ": hash table #" << id << debug_message
                  << "\n";
}

// Version for id argument - checks whether hash_table id exists
static bool start_debug_1_arg(const std::string &function_name,
                              unsigned long id) {
    if (debug)
        std::cerr << function_name << "(" << id << ")\n";
    if (map_of_hashes().find(id) == map_of_hashes().end()) {
        end_debug_message(function_name, id, " does not exist");
        return false;
    }
    return true;
}

//+++++ HASH FUNCTIONS +++++

// HASH_CREATE - Creates hashing table operating on a given hash function.
// Returns id of newly created hash table with hash_function
unsigned long hash_create(hash_function_t hash_function) {
    start_debug_1_arg(__func__, (void *)hash_function);
    static unsigned long id = 0; // The id of the last created hash_table
    while (map_of_hashes().find(id) !=
           map_of_hashes().end()) // Searches for a non-taken id
        id++;
    map_of_hashes().emplace(id, new hash_table_t(0, Hasher(hash_function)));

    end_debug_message(__func__, id, " created");
    return id;
}

// HASH_DELETE - Removes hashing table associated with given id
void hash_delete(unsigned long id) {
    if (!start_debug_1_arg(__func__, id))
        return;
    delete map_of_hashes().find(id)->second;
    map_of_hashes().erase(id);
    end_debug_message(__func__, id, " deleted");
}

// HASH_SIZE - Returns number of elements in hash table associated with given id
// If hashing table doesn't exist, returns 0
size_t hash_size(unsigned long id) {
    if (!start_debug_1_arg(__func__, id)) {
        return 0;
    }
    size_t n = map_of_hashes().find(id)->second->size();
    end_debug_message(__func__, id,
                      " contains " + std::to_string(n) + " element(s)");
    return n;
}

// HASH_INSERT - Inserts given sequence to hash table associated with given id
// Returns true if args are correct and seq wasn't in the table
bool hash_insert(unsigned long id, uint64_t const *seq, size_t size) {
    if (!start_debug_3_args(__func__, id, seq, size))
        return false;
    hash_table_t *table = map_of_hashes().find(id)->second;
    std::vector<uint64_t> vseq(seq, seq + size); // vector that is inserted
    if (table->find(vseq) != table->end()) {     // If vseq is in table
        end_debug_message(__func__, id,
                          ", sequence " + seq_to_string(seq, size) +
                              " was present");
        return false;
    }
    table->insert(vseq);
    end_debug_message(__func__, id,
                      ", sequence " + seq_to_string(seq, size) + " inserted");

    return true;
}

// HASH_REMOVE - Removes seq from the hash table associated with given id
// Returns true if args are correct and seq was in the table
bool hash_remove(unsigned long id, uint64_t const *seq, size_t size) {
    if (!start_debug_3_args(__func__, id, seq, size)) {
        return false;
    }
    auto it = map_of_hashes().find(id);
    hash_table_t *table = it->second;
    std::vector<uint64_t> vseq(seq, seq + size);
    if (table->find(vseq) == table->end()) { // If sequence was not found
        end_debug_message(__func__, id,
                          ", sequence " + seq_to_string(seq, size) +
                              " was not present");
        return false;
    }
    table->erase(vseq);
    end_debug_message(__func__, id,
                      ", sequence " + seq_to_string(seq, size) + " removed");
    return true;
}

// HASH_CLEAR - Removes all existing elements from the given hashing table
void hash_clear(unsigned long id) {
    if (!start_debug_1_arg(__func__, id))
        return;
    hash_table_t *to_be_cleared = map_of_hashes().find(id)->second;
    if (to_be_cleared->size() != 0) { // If to_be_cleared contains anything
        to_be_cleared->clear();
        end_debug_message(__func__, id, " cleared");
    } else {
        end_debug_message(__func__, id, " was empty");
    }
}

// HASH_TEST - Returns logical value of the query: Hashing table associated with
// id exists and contains given sequence of decimals.
bool hash_test(unsigned long id, uint64_t const *seq, size_t size) {
    if (!start_debug_3_args(__func__, id, seq, size))
        return false;
    hash_table_t *table = map_of_hashes().find(id)->second;
    std::vector<uint64_t> vseq(seq, seq + size);
    if (table->find(vseq) == table->end()) { // If sequence not found
        end_debug_message(__func__, id,
                          ", sequence " + seq_to_string(seq, size) +
                              " is not present");
        return false;
    }
    end_debug_message(__func__, id,
                      ", sequence " + seq_to_string(seq, size) + " is present");
    return true;
}
} // namespace jnp1
