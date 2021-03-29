FLEXTRGT = scanner
YACCTRGT = parser
CC = g++
CFLAGS = -std=c++11
BLDSRC += $(FLEXTRGT).cpp $(YACCTRGT).cpp
BLDHEADERS = $(BLDSRC:.cpp=.hpp)
OBJ = $(BLDSRC:.cpp=.o)
dYACC = -v

ifdef dYACC
YFLAG += $(dYACC)
endif

YFLAG += --yacc --defines


all: yacc flex scanner
.PHONY: all

yacc: $(YACCTRGT).y
	bison $(YFLAG) --output=$(YACCTRGT).cpp $<

flex: $(FLEXTRGT).l
	flex --outfile=$(FLEXTRGT).cpp $<

scanner: $(BLDSRC)
	$(CC) $(CFLAGS) -o $@ $(BLDSRC)

clean:
	rm -rf $(FLEXTRGT) $(BLDHEADERS) $(OBJ) $(BLDSRC) *.output