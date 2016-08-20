#pragma once
#include <stdint.h>

/**
 * \brief Contains the possible module types the kernel supports.
 */
enum class ModType : int {
    /// Contains nothing
    none,
    /// Contains a module for ouputting text
    output_text,
};
//Some typedefs to make the code look cleaner.
typedef void(**table_type)();
typedef ModType(*getType_type)();
typedef bool(*spawnAt_type)(void*);
typedef size_t(*sizeof_type)();
/**
 * \brief returns the function table (3 entries)
 * 
 * The function table contains 3 entries in the following order:
 * ModType getType();
 * size_t size_of();
 * bool spawnAt(void*);
 */
extern "C" table_type getTable(void*(*)(ModType));

/**
 * \brief returns the type of the module
 */
auto getType() -> ModType;

/**
 * \brief spawns a module at a specified address
 * returns true if successful
 */
auto spawnAt(void*) -> bool;

/**
 * \brief size of the module object
 */
auto size_of() -> size_t;