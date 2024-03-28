#include "page_replacement.h"
#include <algorithm> // For std::min_element
#include <iostream>

PageReplacement::PageReplacement(unsigned int bitstringUpdateInterval) 
    : currentTime(0), updateInterval(bitstringUpdateInterval) {}

void PageReplacement::accessPage(unsigned int pageNumber) {
    // Increment access frequency or add new page if it does not exist
    bool pageFound = false;
    for(auto &page : pages) {
        if(page.pageNumber == pageNumber) {
            page.accessFrequency = std::min(page.accessFrequency + 1, (1U << 16) - 1); // Prevent overflow
            page.lastAccessTime = currentTime;
            pageFound = true;
            break;
        }
    }
    if(!pageFound) {
        pages.push_back(Page(pageNumber));
    }
    currentTime++;
    if (currentTime % updateInterval == 0) {
        agePages();
    }
}

std::optional<unsigned int> PageReplacement::replacePage() {
    if(pages.empty()) {
        return std::nullopt;
    }
    // Find the page with the lowest frequency for replacement
    auto victim = std::min_element(pages.begin(), pages.end(), [](const Page &a, const Page &b) {
        return a.accessFrequency < b.accessFrequency || (a.accessFrequency == b.accessFrequency && a.lastAccessTime < b.lastAccessTime);
    });
    unsigned int victimPageNumber = victim->pageNumber;
    pages.erase(victim); // Remove the victim page from the list
    return victimPageNumber; // Return the page number of the replaced (victim) page
}

void PageReplacement::agePages() {
    // Periodically called to age the pages
    for(auto &page : pages) {
        page.accessFrequency >>= 1; // Right shift to simulate aging
    }
}