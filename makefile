# Makefile for the scheduler project

TARGET = pagingwithpr
SOURCES = log_helpers.c main.cpp pagetable.cpp page_replacement.cpp vaddr_tracereader.cpp
HEADERS = log_helpers.h pagetable.h page_replacement.h vaddr_tracereader.h

CXX = g++
CXXFLAGS = -std=c++11 -Wall

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)

