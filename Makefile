PREFIX = /usr/local
dbg = -g
CC=g++
CFLAGS=-fomit-frame-pointer -maes -std=c++11 -msse4.2 -fno-strict-aliasing -pedantic -Wall -Wextra -Wunreachable-code -Wmissing-declarations -Wunused-function -Wno-overlength-strings -Wno-deprecated-declarations -O3
LDFLAGS= -L/usr/lib/x86_64-linux-gnu -Wl,-Bstatic -lcrypto -Wl,-Bdynamic -ldl -lssl -lgmpxx -lgmp -lpthread
SOURCES= prg-test.cpp prg.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=testprg

ifeq ($(AESNI), 1)
    CFLAGS+=-DAESNI
endif

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -f *.o $(LIB) $(OBJECTS) $(EXECUTABLE)