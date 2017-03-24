CXXFLAGS ?= -std=c++14 -Wall -Wextra
CFLAGS ?= -std=c11 -Wall -Wextra
LDFLAGS += -lmvas

TARGET = mvastest
SOURCES = menu.cc benchmarks.cc interactive.cc main.cc

all: ${TARGET}

mvastest: ${SOURCES}
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: ${TARGET}
	./${TARGET}

clean:
	rm -f ${TARGET}

.PHONY: clean
