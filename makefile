# Makefile for the demand paging simulation project

TARGET = pagingwithpr
SOURCES = src/main.cpp src/pagetable.cpp src/page_replacement.cpp src/vaddr_tracereader.cpp src/log_helpers.c
HEADERS = include/pagetable.h include/page_replacement.h include/vaddr_tracereader.h include/log_helpers.h

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

$(TARGET): $(SOURCES) $(HEADERS)
    $(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

.PHONY: clean
clean:
    rm -f $(TARGET)
