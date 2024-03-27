#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <vector>
#include <optional>

struct Page {
    unsigned int pageNumber;
    unsigned int accessFrequency;
    unsigned long lastAccessTime;
    Page(unsigned int pageNum) : pageNumber(pageNum), accessFrequency(0), lastAccessTime(0) {}
};

class PageReplacement {
public:
    PageReplacement();
    void accessPage(unsigned int pageNumber);
    std::optional<unsigned int> replacePage(); // Returns the page number of the replaced page, if any
    void agePages();
private:
    std::vector<Page> pages;
    unsigned long currentTime;
};

#endif

