TARGET=scanner
CC=g++
CFLAGS=-std=c++11

all: flex scanner
.PHONY: all

flex: $(TARGET).l
	flex --outfile=$(TARGET).cpp $<

scanner: $(TARGET).cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET) $(TARGET).h $(TARGET).cpp