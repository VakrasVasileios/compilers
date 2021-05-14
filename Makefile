FLEXTRGT = scanner
YACCTRGT = parser
CC = g++
CFLAGS = -std=c++11
SRC +=  src/*.cpp \
		src/expression/*.cpp \
		src/expression/constant/*.cpp \
		src/expression/constant/numeric_constant/*.cpp \
		src/expression/symbol/*.cpp \
		src/expression/symbol/variable/*.cpp \
		src/expression/symbol/function/*.cpp 

BLDSRC += $(FLEXTRGT).cpp $(YACCTRGT).cpp
BLDHEADERS = $(BLDSRC:.cpp=.hpp)
OBJ += $(BLDSRC:.cpp=.o) $(SRC:.cpp=.o)
#dYACC = -v
GDBenable = -g
# myLog = -D DELOG

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