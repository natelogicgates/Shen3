# Makefile for the demand paging simulation project

TARGET = pagingwithpr
SOURCES = main.cpp pagetable.cpp page_replacement.cpp vaddr_tracereader.cpp log_helpers.c
HEADERS = pagetable.h page_replacement.h vaddr_tracereader.h log_helpers.h

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

$(TARGET): $(SOURCES) $(HEADERS)
    $(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

.PHONY: clean
clean:
    rm -f $(TARGET)
