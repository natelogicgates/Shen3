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
    PageReplacement(unsigned int bitstringUpdateInterval);
    void accessPage(unsigned int pageNumber);
    std::optional<unsigned int> replacePage(); // Returns the page number of the replaced page, if any
    void agePages();
private:
    std::vector<Page> pages;
    unsigned long currentTime;
    unsigned int updateInterval;
    void updateAccessHistory();
};

#endif // PAGE_REPLACEMENT_H