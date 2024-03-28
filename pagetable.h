#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <vector>
#include <optional>

class PageTableEntry {
public:
    bool valid = false;
    unsigned int frameNumber = 0;
    PageTableEntry* nextLevel = nullptr; // Points to the next level (if not a leaf node)
};

class Level {
public:
    std::vector<PageTableEntry> entries;
    Level(int size);
};

class PageTable {
public:
    PageTable(int levels, int entriesPerLevel);
    bool insert(unsigned int virtualAddress, unsigned int frameNumber);
    std::optional<unsigned int> search(unsigned int virtualAddress);
    bool remove(unsigned int virtualAddress);
private:
    std::vector<Level> tableLevels;
    int entriesPerLevel;
    unsigned int extractBits(unsigned int value, int startBit, int numBits);
};

#endif // PAGETABLE_H