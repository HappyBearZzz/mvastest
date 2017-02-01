CXXFLAGS ?= -std=c++14 -Wall -Wextra
CFLAGS ?= -std=c11 -Wall -Wextra
LDFLAGS += -lmvas

TARGET = vastests
SOURCES = benchmarks.cc interactive.cc main.cc

all: ${TARGET}

vastests: ${SOURCES}
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
