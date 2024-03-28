#include "page_replacement.h"
#include <algorithm> // For std::min_element
#include <iostream>

PageReplacement::PageReplacement(unsigned int maxFrames, unsigned int bitstringUpdateInterval)
    : currentTime(0), updateInterval(bitstringUpdateInterval), maxFrames(maxFrames), allocatedFrames(0) {}

void PageReplacement::accessPage(unsigned int pageNumber) {
    bool pageFound = false;
    for (auto &page : pages) {
        if (page.pageNumber == pageNumber) {
            page.accessFrequency = std::min(page.accessFrequency + 1, (1U << 16) - 1); // Prevent overflow
            page.lastAccessTime = currentTime;
            pageFound = true;
            break;
        }
    }
    if (!pageFound) {
        if (isFull()) {
            auto frameNumber = replacePage().value(); // Get the frame number of the page to be replaced
            pages.push_back(Page(pageNumber, frameNumber)); // Use the same frame for the new page
        } else {
            pages.push_back(Page(pageNumber, allocatedFrames)); // Allocate a new frame
            allocatedFrames++;
        }
    }
    currentTime++;
    if (currentTime % updateInterval == 0) {
        agePages();
    }
}

std::optional<unsigned int> PageReplacement::replacePage() {
    if (pages.empty()) {
        return std::nullopt;
    }
    auto victim = std::min_element(pages.begin(), pages.end(), [](const Page &a, const Page &b) {
        return a.accessFrequency < b.accessFrequency || (a.accessFrequency == b.accessFrequency && a.lastAccessTime < b.lastAccessTime);
    });
    unsigned int frameNumber = victim->frameNumber;
    pages.erase(victim); // Remove the victim page from the list
    return frameNumber; // Return the frame number of the replaced (victim) page
}

void PageReplacement::agePages() {
    for (auto &page : pages) {
        page.accessFrequency >>= 1; // Right shift to simulate aging
    }
}

bool PageReplacement::isFull() const {
    return allocatedFrames >= maxFrames;
}

unsigned int PageReplacement::allocateFrame(unsigned int vpn) {
    if (isFull()) {
        auto frameNumber = replacePage().value(); // Get the frame number of the page to be replaced
        pages.push_back(Page(vpn, frameNumber)); // Use the same frame for the new page
        return frameNumber;
    } else {
        pages.push_back(Page(vpn, allocatedFrames)); // Allocate a new frame
        return allocatedFrames++;
    }
}
