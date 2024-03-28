#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <vector>
#include <optional>

class Page {
public:
    unsigned int pageNumber;
    unsigned int accessFrequency;
    unsigned long lastAccessTime;
    Page(unsigned int pageNum) : pageNumber(pageNum), accessFrequency(1 << 15), lastAccessTime(0) {}
};

class PageReplacement {
public:
    PageReplacement(unsigned int maxFrames, unsigned int bitstringUpdateInterval);
    bool isFull() const;
    std::optional<unsigned int> replacePage();
    unsigned int allocateFrame(unsigned int vpn);
    void accessPage(unsigned int pageNumber);
    void agePages();

private:
    std::vector<Page> pages;
    unsigned long currentTime;
    unsigned int updateInterval;
    unsigned int maxFrames;
    unsigned int allocatedFrames; // Added to track the number of allocated frames
};

#endif // PAGE_REPLACEMENT_H
