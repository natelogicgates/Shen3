// In page_replacement.h
#include <vector>
#include <optional>

class Page {
public:
    unsigned int pageNumber;
    unsigned int accessFrequency;
    unsigned long lastAccessTime;
    Page(unsigned int pageNum);
};

class PageReplacement {
public:
    PageReplacement(unsigned int maxFrames, unsigned int bitstringUpdateInterval);
    bool isFull() const; // Check if the frame list is full
    std::optional<unsigned int> replacePage(); // Choose a page to replace
    unsigned int allocateFrame(unsigned int vpn); // Allocate a frame for a VPN
    void accessPage(unsigned int pageNumber); // Mark a page as accessed
    void agePages(); // Age the pages for the replacement algorithm

private:
    std::vector<Page> pages;
    unsigned long currentTime;
    unsigned int updateInterval;
    unsigned int maxFrames; // Maximum number of frames
    void updateAccessHistory();
};
