#include "page_replacement.h"

PageReplacement::PageReplacement() : currentTime(0) {}

void PageReplacement::accessPage(unsigned int pageNumber) {
    // Increment access frequency or add new page if it does not exist
    bool pageFound = false;
    for(auto &page : pages) {
        if(page.pageNumber == pageNumber) {
            page.accessFrequency++;
            page.lastAccessTime = currentTime++;
            pageFound = true;
            break;
        }
    }
    if(!pageFound) {
        pages.push_back(Page(pageNumber));
    }
}

std::optional<unsigned int> PageReplacement::replacePage() {
    // Implement NFU with aging. Return the replaced page number
    if(pages.empty()) {
        return {};
    }
    // Example: Find the page with the lowest frequency for replacement
    auto victim = std::min_element(pages.begin(), pages.end(), [](const Page &a, const Page &b) {
        return a.accessFrequency < b.accessFrequency || (a.accessFrequency == b.accessFrequency && a.lastAccessTime < b.lastAccessTime);
    });
    unsigned int victimPageNumber = victim->pageNumber;
    pages.erase(victim);
    return victimPageNumber;
}

void PageReplacement::agePages() {
    // Periodically called to age the pages
    for(auto &page : pages) {
        page.accessFrequency >>= 1; // Right shift to simulate aging
    }
}
