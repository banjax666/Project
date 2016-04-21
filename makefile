# /*
# -- Group Number: 68
# -- Name: Akhilesh Sudhakar  ID: 2013A7PS173P
# -- Name: Shubham Sharma     ID: 2013A7PS110P
# */
CC = gcc
CFLAGS =-g
RM = rm -rf
OBJECTS = driver.o lexer.o parser.o populateSymbolTable.o semanticAnalysis.o symbolTableHash.o

.PHONY: all
all: toycompiler

toycompiler:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

driver.o: lexer.h parser.h
lexer.o: lexerDef.h lexer.h
parser.o: parserDef.h lexerDef.h lexer.h
populateSymbolTable.o: populateSymbolTable.h
semanticAnalysis.o: semanticAnalysis.h
symbolTableHash.o: symbolTableHash.h lexerDef.h parserDef.h

.PHONY: install
install:
	$(RM) $(OBJECTS)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) toycompiler
