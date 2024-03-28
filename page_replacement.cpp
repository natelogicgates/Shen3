#include "page_replacement.h"
#include <algorithm> // For std::min_element
#include <iostream>

PageReplacement::PageReplacement(unsigned int bitstringUpdateInterval, unsigned int numFrames) 
    : currentTime(0), updateInterval(bitstringUpdateInterval), frameCount(numFrames), allocatedFrames(0) {}


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
bool PageReplacement::isFull() const {
    return allocatedFrames >= maxFrames;
}

unsigned int PageReplacement::allocateFrame(unsigned int vpn) {
    if (isFull()) {
        throw std::runtime_error("No free frames available");
    } else {
        allocatedFrames++;
        auto evictedVpn = replacePage();
        // Find and update the evicted page with the new VPN
        for (auto &page : pages) {
            if (page.pageNumber == evictedVpn.value()) {
                page.pageNumber = vpn;
                return page.frameNumber; // Reuse the frame of the evicted page
            }
        }
      return allocatedFrames - 1;  
    }

unsigned int PageReplacement::evictPage() {
    if (pages.empty()) {
        throw std::runtime_error("No pages to evict");
    }
    auto victim = std::min_element(pages.begin(), pages.end(), [](const Page &a, const Page &b) {
        return a.accessFrequency < b.accessFrequency || (a.accessFrequency == b.accessFrequency && a.lastAccessTime < b.lastAccessTime);
    });
    unsigned int victimPageNumber = victim->pageNumber;
    pages.erase(victim);
    allocatedFrames--; // Assume frame is now free
    return victimPageNumber;
}
