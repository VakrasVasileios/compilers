FLEXTRGT = scanner
YACCTRGT = parser
CC = g++
CFLAGS = -std=c++11
SRC += src/parser_manager/*.cpp \
		src/block/block.cpp \
		src/program_stack/program_stack.cpp \
		src/symbol_table/*.cpp \
		src/symbol_table/symbol_table_entry/*.cpp \
		src/symbol_table/symbol_table_entry/function_entry/*.cpp \
		src/symbol_table/symbol_table_entry/variable_entry/*.cpp \
		src/expression/*.cpp
BLDSRC += $(FLEXTRGT).cpp $(YACCTRGT).cpp
BLDHEADERS = $(BLDSRC:.cpp=.hpp)
OBJ += $(BLDSRC:.cpp=.o) $(SRC:.cpp=.o)
dYACC = -v
GDBenable = -g
#myLog = -DELOG

ifdef dYACC
YFLAG += $(dYACC)
endif

ifdef myLog
CFLAGS += $(myLog)
endif

ifdef GDBenable
CFLAGS += $(GDBenable)
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