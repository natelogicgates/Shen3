#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <iomanip>
#include "pagetable.h"
#include "page_replacement.h"

int main(int argc, char* argv[]) {
    std::string traceFilePath;
    std::vector<int> bitsPerLevel;
    int numFrames = 999999; // Default value for an almost unlimited number of frames
    unsigned int bitstringUpdateInterval = 10; // Default aging interval
    bool traceFileProvided = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f") {
            numFrames = std::stoi(argv[++i]);
        } else if (arg == "-b") {
            bitstringUpdateInterval = std::stoi(argv[++i]);
        } else if (std::isdigit(arg[0])) {
            bitsPerLevel.push_back(std::stoi(arg));
        } else {
            traceFilePath = arg;
            traceFileProvided = true;
        }
    }

    if (!traceFileProvided) {
        std::cerr << "Error: Trace file path must be specified.\n";
        return -1;
    }

    PageTable pageTable(bitsPerLevel);
    PageReplacement pageReplacement(numFrames, bitstringUpdateInterval);

    std::ifstream traceFile(traceFilePath);
    if (!traceFile.is_open()) {
        std::cerr << "Error: Unable to open trace file: " << traceFilePath << "\n";
        return -1;
    }

    unsigned int addressesProcessed = 0, pageHits = 0, pageMisses = 0, pageReplacements = 0;
    std::string line;

    while (std::getline(traceFile, line)) {
        addressesProcessed++;
        unsigned int virtualAddress = std::stoul(line, nullptr, 16);

        // Calculate the VPN from the virtual address
        unsigned int vpn = virtualAddress >> (32 - std::accumulate(bitsPerLevel.begin(), bitsPerLevel.end(), 0));

        auto searchResult = pageTable.search(virtualAddress);
        if (searchResult.has_value()) {
            // Page hit
            pageHits++;
            pageReplacement.accessPage(vpn); // Update access frequency and last access time
        } else {
            // Page miss
            pageMisses++;
            if (!pageReplacement.isFull()) {
                // Allocate new frame
                unsigned int frameNumber = pageReplacement.allocateFrame(vpn);
                pageTable.insert(virtualAddress, frameNumber);
            } else {
                // Page replacement
                pageReplacements++;
                auto evictedFrameNumber = pageReplacement.replacePage();
                if (evictedFrameNumber.has_value()) {
                    // Remove the old mapping and add the new one
                    pageTable.remove(virtualAddress); // This might not be necessary depending on eviction logic
                    pageTable.insert(virtualAddress, evictedFrameNumber.value());
                } else {
                    std::cerr << "Error: Failed to evict a page.\n";
                    return -1;
                }
            }
        }
    }

    // Output simulation statistics
    std::cout << "Page size: " << (1 << std::accumulate(bitsPerLevel.begin(), bitsPerLevel.end(), 0)) << " bytes\n";
    std::cout << "Frames allocated: " << numFrames << "\n";
    std::cout << "Addresses processed: " << addressesProcessed << "\n";
    std::cout << "Page hits: " << pageHits << ", Misses: " << pageMisses << "\n";
    std::cout << "Page Replacements: " << pageReplacements << "\n";
    std::cout << "Page hit percentage: " << std::fixed << std::setprecision(2) << (static_cast<double>(pageHits) / addressesProcessed) * 100.0 << "%\n";
    std::cout << "Miss percentage: " << std::fixed << std::setprecision(2) << (static_cast<double>(pageMisses) / addressesProcessed) * 100.0 << "%\n";

    return 0;
}
