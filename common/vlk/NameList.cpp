// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>
#include <algorithm>
#include <stdexcept>

// Local includes
#include "NameList.h"

using namespace std;

void NameList::Clear()
{
    names.clear();
    pointers.clear();
}

void NameList::AddName(const std::string & name)
{
    if (HasName(name)) {
        throw std::logic_error("Name '" 
            + name + "' has already been added to name list.");
    }

    names.push_back(name);
}

bool NameList::HasName(const std::string & name) const noexcept
{
    return find(names.begin(), names.end(), name) != names.end();
}

uint32_t NameList::NumNames() const
{
    return static_cast<uint32_t>(names.size());
}

const std::vector<const char*> & NameList::GetPointers() const
{
    pointers.clear();
    for (const auto & s : names) {
        pointers.push_back(s.c_str());
    }

    return pointers;
}

const char* const * NameList::GetPointerArray() const {    
    return names.empty() ? nullptr : GetPointers().data();
}
