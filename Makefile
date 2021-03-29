FLEXTRGT = scanner
#YACCTRGT = parser
CC = g++
CFLAGS = -std=c++11
SRC += $(FLEXTRGT).cpp #$(YACCTRGT).cpp
OBJ = $(SRC:.cpp=.o)

all: flex scanner
.PHONY: all

#yacc: $(YACCTRGT).y
#	bison --yacc --defines --output=$(YACCTRGT).cpp $<

flex: $(FLEXTRGT).l
	flex --outfile=$(FLEXTRGT).cpp $<

scanner: $(SRC)
	$(CC) $(CFLAGS) -o calc $<

clean:
	rm -rf $(FLEXTRGT) $(FLEXTRGT).h $(OBJ) $(SRC)