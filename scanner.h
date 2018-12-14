#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <cstring>
#include "token.h"


using namespace std;

bool issymbol(char c);
Token scanner(ifstream &inFile);
bool isComment(char c);
bool isEOF(char c);
bool iskeyword(string);
Token exitToken(int, string,int );
int getTableRow(int, int,int,string);
int getColumn(char c);



#endif 
