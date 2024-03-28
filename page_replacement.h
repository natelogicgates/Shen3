#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <vector>
#include <optional>

class Page {
public:
    unsigned int pageNumber;
    unsigned int accessFrequency;
    unsigned long lastAccessTime;
    unsigned int frameNumber; // Assuming each Page has a frameNumber for direct mapping
    Page(unsigned int pageNum, unsigned int frameNum) 
        : pageNumber(pageNum), accessFrequency(1 << 15), lastAccessTime(0), frameNumber(frameNum) {}
};

class PageReplacement {
public:
    PageReplacement(unsigned int maxFrames, unsigned int bitstringUpdateInterval);
    bool isFull() const;
    std::optional<unsigned int> replacePage(); // Returns the frame number of the replaced page
    unsigned int allocateFrame(unsigned int vpn); // Allocates a frame to a VPN, returns the frame number
    void accessPage(unsigned int pageNumber);
    void agePages();

private:
    std::vector<Page> pages;
    unsigned long currentTime;
    unsigned int updateInterval;
    unsigned int maxFrames;
    unsigned int allocatedFrames; // Tracks the number of allocated frames
};

#endif // PAGE_REPLACEMENT_H
