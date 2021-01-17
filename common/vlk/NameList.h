// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <cstdint>
#include <string>
#include <vector>

/**
 * Utility class whichs maintails strings and a list of
 * pointesr to these strings.
 * 
 * This class is for easy use of the Vulkan API, which
 * often requires an array of pointers to null-terminated
 * c-strings.
 */
class NameList {
public:

    /**
     * Clears this list.
     */
    void Clear();

    /**
     * Adds a name to this list.
     * 
     * @param name  the name to add
     */
    void AddName(const std::string & name);

    /**
     * Tests whether a name has already been added to this list.
     * 
     * @param name  the name to test
     * @return `true` if the name has already beed added
     */
    bool HasName(const std::string & name) const noexcept;

    /**
     * Returns a list with pointers to the names in this list.
     * 
     * @return the list with pointers
     */ 
    const std::vector<const char*> & GetPointers() const;

    /**
     * Returns pointer to an array of null-terminated c-strings.
     * 
     * @return pointer to c-string array.
     */
    const char* const * GetPointerArray() const;

    /**
     * Returns the number of names in this list.
     * 
     * @return the number of names;
     */
    uint32_t NumNames() const;

private:
    /** The names of this list. */
    std::vector<std::string> names;

    /** The list of pointer to the names. */
    mutable std::vector<const char*> pointers;
};