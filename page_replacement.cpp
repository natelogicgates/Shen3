#include "pagetable.h"

Level::Level(int size) : entries(size) {}

PageTable::PageTable(int levels, int entriesPerLevel) : entriesPerLevel(entriesPerLevel) {
    for(int i = 0; i < levels; i++) {
        tableLevels.emplace_back(Level(entriesPerLevel));
    }
}

bool PageTable::insert(unsigned int virtualAddress, unsigned int frameNumber) {
    // Logic to determine the correct level and index for the virtual address
    // and set the frame number
    return true; // Placeholder
}

std::optional<unsigned int> PageTable::search(unsigned int virtualAddress) {
    // Logic to search for the virtual address in the table
    // and return the corresponding frame number if found
    return {}; // Placeholder
}

bool PageTable::remove(unsigned int virtualAddress) {
    // Logic to remove an entry for the given virtual address
    return true; // Placeholder
}

unsigned int PageTable::extractBits(unsigned int value, int startBit, int numBits) {
    return (value >> startBit) & ((1 << numBits) - 1);
}
