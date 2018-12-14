#ifndef CODEGENERATION_h
#define CODEGENERATION_h
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include "node.h"
using namespace std;
void codeGenerate(Node *root, fstream &outFile);
void generate(Node *root,fstream &outFile);
void error(string label,string token);





#endif