CC = g++

CFLAGS = -g -Wall

TARGET = comp

OBJ = main.o parser.o scanner.o testTree.o codeGeneration.o

stats : $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o : main.cpp token.h parser.h scanner.h testTree.h codeGeneration.h
	$(CC) $(CFLAGS) -c main.cpp
parser.o : parser.cpp parser.h token.h scanner.h testTree.h codeGeneration.h
	$(CC) $(CFLAGS) -c parser.cpp
scanner.o : scanner.cpp scanner.h token.h parser.h testTree.h codeGeneration.h
	$(CC) $(CFLAGS) -c scanner.cpp
testTree.o : testTree.cpp testTree.h token.h parser.h scanner.h node.h codeGeneration.h
	$(CC) $(CFLAGS) -c testTree.cpp
codeGeneration.o : codeGeneration.cpp codeGeneration.h token.h parser.h scanner.h node.h
	$(CC) $(CFLAGS) -c codeGeneration.cpp
clean:
	/bin/rm -f *.o $(TARGET)
