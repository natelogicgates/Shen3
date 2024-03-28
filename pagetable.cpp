#include "pagetable.h"
#include <cmath>
#include <iostream>

PageTable::PageTable(const std::vector<int>& bitsPerLevel) : root(new PageTableEntry()), offsetBits(32) {
    calculateMasksAndShifts(bitsPerLevel);
}

PageTable::~PageTable() {
    deleteSubtree(root);
}

bool PageTable::insert(unsigned int virtualAddress, unsigned int frameNumber) {
    auto entry = navigateToEntry(virtualAddress, true);
    if (entry) {
        entry->frameNumber = frameNumber;
        return true;
    }
    return false;
}

std::optional<unsigned int> PageTable::search(unsigned int virtualAddress) const {
    auto entry = navigateToEntry(virtualAddress, false);
    return entry ? entry->frameNumber : std::nullopt;
}

bool PageTable::remove(unsigned int virtualAddress) {
    auto entry = navigateToEntry(virtualAddress, false);
    if (entry && entry->frameNumber) {
        entry->frameNumber = std::nullopt;
        return true;
    }
    return false;
}
bool PageTable::hasMapping(unsigned int vpn) const {
    return search(vpn).has_value();
}

void PageTable::addMapping(unsigned int vpn, unsigned int frameNumber) {
    insert(vpn, frameNumber);
}

void PageTable::removeMapping(unsigned int vpn) {
    remove(vpn);
}

PageTableEntry* PageTable::navigateToEntry(unsigned int virtualAddress, bool createIfMissing) const {
    PageTableEntry* current = root;
    for (int level = 0; level < masks.size(); ++level) {
        unsigned int index = (virtualAddress & masks[level]) >> shiftAmounts[level];
        if (!current->nextLevel) {
            if (!createIfMissing) return nullptr;
            current->nextLevel = new PageTableEntry[std::pow(2, masks.size() - level - 1)];
        }
        current = &current->nextLevel[index];
    }
    return current;
}

void PageTable::calculateMasksAndShifts(const std::vector<int>& bitsPerLevel) {
    int shiftAmount = offsetBits;
    for (auto bits : bitsPerLevel) {
        shiftAmount -= bits;
        masks.push_back((1 << bits) - 1 << shiftAmount);
        shiftAmounts.push_back(shiftAmount);
    }
}

void PageTable::deleteSubtree(PageTableEntry* entry) {
    if (!entry) return;
    if (entry->nextLevel) {
        delete[] entry->nextLevel;
    }
    delete entry;
}
