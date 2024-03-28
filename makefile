# Makefile for the scheduler project

TARGET = pagingwithpr
SOURCES = log_helpers.c main.cpp pagetable.cpp page_replacement.cpp
HEADERS = log_helpers.h pagetable.h page_replacement.h

CXX = g++
CXXFLAGS = -std=c++17 -Wall

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)

