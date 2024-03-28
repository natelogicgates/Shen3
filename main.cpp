#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> 
#include <iomanip> 
#include "PageTable.h" 
#include "PageReplacement.h" 

int main(int argc, char* argv[]) {
    std::string traceFilePath;
    std::vector<int> bitsPerLevel; // To store the number of bits for each level
    int numFrames = 999999; // Default: simulate an infinite number of frames unless specified
    int pageSize = 4096; // Default page size in bytes
    bool traceFileProvided = false;

    // Example of parsing additional arguments
    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "-f") {
            numFrames = std::stoi(argv[++i]);
        } else if(arg == "-p") {
            pageSize = std::stoi(argv[++i]);
        } else if(std::isdigit(arg[0])) {
            bitsPerLevel.push_back(std::stoi(arg));
        } else {
            traceFilePath = arg;
            traceFileProvided = true;
        }
    }

    if (!traceFileProvided) {
        std::cerr << "Error: Trace file path must be specified." << std::endl;
        return -1;
    }

    // Initialize page table and page replacement classes
    PageTable pageTable(bitsPerLevel);
    PageReplacement pageReplacement(numFrames);

    std::ifstream traceFile(traceFilePath);
    if (!traceFile.is_open()) {
        std::cerr << "Error: Unable to open trace file: " << traceFilePath << std::endl;
        return -1;
    }

    unsigned int addressesProcessed = 0, pageHits = 0, pageMisses = 0, pageReplacements = 0;
    std::string line;

    while(std::getline(traceFile, line)) {
        addressesProcessed++;
        unsigned int virtualAddress = std::stoul(line, nullptr, 16);
        auto vpn = pageTable.virtualAddressToPageNumber(virtualAddress, pageSize);

        if(pageTable.hasMapping(vpn)) {
            pageHits++;
        } else {
            pageMisses++;
            if(!pageReplacement.isFull()) {
                unsigned int newFrame = pageReplacement.allocateFrame(vpn);
                pageTable.addMapping(vpn, newFrame);
            } else {
                unsigned int evictedVpn = pageReplacement.evictPage();
                pageTable.removeMapping(evictedVpn);
                unsigned int newFrame = pageReplacement.allocateFrame(vpn);
                pageTable.addMapping(vpn, newFrame);
                pageReplacements++;
            }
        }
    }

    // Output simulation statistics
    std::cout << "Page size: " << pageSize << " bytes\n";
    std::cout << "Frames allocated: " << numFrames << "\n";
    std::cout << "Addresses processed: " << addressesProcessed << "\n";
    std::cout << "Page hits: " << pageHits << ", Misses: " << pageMisses << "\n";
    std::cout << "Page Replacements: " << pageReplacements << "\n";
    std::cout << "Page hit percentage: " << std::setprecision(2) << static_cast<double>(pageHits) / addressesProcessed * 100.0 << "%\n";
    std::cout << "Miss percentage: " << std::setprecision(2) << static_cast<double>(pageMisses) / addressesProcessed * 100.0 << "%\n";

    return 0;
}
