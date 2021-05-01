FLEXTRGT = scanner
YACCTRGT = parser
CC = g++
CFLAGS = -std=c++11
SRC += src/parser_manager/*.cpp src/block/Block.cpp src/program_stack/ProgramStack.cpp src/symbol_table/*.cpp src/symbol_table/symbol_table_entry/*.cpp
BLDSRC += $(FLEXTRGT).cpp $(YACCTRGT).cpp
BLDHEADERS = $(BLDSRC:.cpp=.hpp)
OBJ += $(BLDSRC:.cpp=.o) $(SRC:.cpp=.o)
dYACC = -v
#myLog = -DELOG

ifdef dYACC
YFLAG += $(dYACC)
endif

ifdef myLog
CFLAGS += $(myLog)
endif

YFLAG += --yacc --defines


all: yacc flex scanner
.PHONY: all

yacc: $(YACCTRGT).y
	bison $(YFLAG) --output=$(YACCTRGT).cpp $<

flex: $(FLEXTRGT).l
	flex --outfile=$(FLEXTRGT).cpp $<

scanner: $(BLDSRC)
	$(CC) $(CFLAGS) -o $@ $(BLDSRC) $(SRC)

clean:
	rm -rf $(FLEXTRGT) $(BLDHEADERS) $(OBJ) $(BLDSRC) *.output