#include <vector>
#include <optional>

struct PageTableEntry {
    std::optional<unsigned int> frameNumber = std::nullopt;
    PageTableEntry* nextLevel = nullptr;
};

class PageTable {
public:
    PageTable(const std::vector<int>& bitsPerLevel);
    ~PageTable();
    bool insert(unsigned int virtualAddress, unsigned int frameNumber);
    std::optional<unsigned int> search(unsigned int virtualAddress) const;
    bool remove(unsigned int virtualAddress);
    bool hasMapping(unsigned int vpn) const; // Check if a VPN is mapped
    void addMapping(unsigned int vpn, unsigned int frameNumber); // Map VPN to PFN
    bool removeMapping(unsigned int vpn); // Remove a VPN to PFN mapping

private:
    PageTableEntry* root;
    std::vector<int> shiftAmounts;
    std::vector<unsigned int> masks;
    int offsetBits;

    PageTableEntry* navigateToEntry(unsigned int virtualAddress, bool createIfMissing) const;
    void calculateMasksAndShifts(const std::vector<int>& bitsPerLevel);
    static void deleteSubtree(PageTableEntry* entry);
};
