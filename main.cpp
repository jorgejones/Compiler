#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <cstring>
#include "token.h"
#include "parser.h"
#include "scanner.h"
#include "testTree.h"
#include "codeGeneration.h"
#include "node.h"
//header files

using namespace std; //for std use



int main(int argc, char * argv[])
{
	string str;
	char value;
	int num = 1;
	ifstream inFile; //input file stream called inFile
	string fileName;
	fstream outFile;
	//string targetName;
//input validation
    if(argc > 2){
        printf("Too many Arguments");
        exit(0);
    }
    if(argc == 2){
        fileName = argv[1];
        fileName = fileName + ".fs18";
        inFile.open(fileName.c_str());
	}
     if(argc == 1) {
		 //ofstream outFile;
		 //outFile.open("output.txt");
		 //fileName = "output.txt";
		 //string str;
		 string message;
		 //int num = 1;
		 while (!cin.eof()){


		 }
		 if (!inFile) {
			 cout << argv[1] << " file does not exist!\n";
			 return -1;
		 }

//variable for parsing the lines, strings, and characters of a file
	 }

//
		 if (inFile) {
			 outFile.open("file.asm");
		 	//FILE fp* = outFile;
		 	//parser(inFile);
		 	 Node *tree = new Node;
			 tree = parser(inFile);

			 int depth = 0;
			//printTree(tree,depth);
			 //recGen(tree,outFile);
			 //generateCode(tree, outFile);
			 codeGenerate(tree,outFile);
		 }

inFile.close();
outFile.close();
//remove("out.text");

return 0;

}
