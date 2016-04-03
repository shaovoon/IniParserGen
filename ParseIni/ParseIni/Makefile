# Project: MiniCSV

#CPP      = clang++ -stdlib=libstdc++ -lstdc++ -std=c++11
CPP      = g++-4.7 -lstdc++ -std=c++11
CC       = gcc
OBJ      = ParseIni.o $(RES)
LINKOBJ  = ParseIni.o $(RES)
BIN      = ParseIni
CXXFLAGS = -Wall -g -O1 
CFLAGS   = -Wall -g -O1  
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before ParseIni all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "ParseIni" $(LIBS)

ParseIni.o: ParseIni.cpp
	$(CPP) -c ParseIni.cpp -o ParseIni.o $(CXXFLAGS)
