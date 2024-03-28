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
    PageReplacement(unsigned int bitstringUpdateInterval, unsigned int numFrames);
    void accessPage(unsigned int pageNumber);
    std::optional<unsigned int> replacePage();
    void agePages();
    bool isFull() const;
    unsigned int allocateFrame(unsigned int vpn);
    unsigned int evictPage();

private:
    std::vector<Page> pages;
    unsigned long currentTime;
    unsigned int updateInterval;
    unsigned int frameCount; // Total number of frames
    unsigned int allocatedFrames; // Count of allocated frames

    void updateAccessHistory();
};

#endif // PAGE_REPLACEMENT_H
