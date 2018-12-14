#ifndef PARSER_H
#define PARSER_H
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



extern int TokenTable[4][5];

Node* parser(ifstream &inFile);
void error(Token Tk);
bool verify(string);
bool insert(string);
bool insertLocal(string);
void remove(int);
void push(string);
void pop();
int find(string);
Node* program(ifstream &inFile);
Node* vars(ifstream &inFile);
Node* expr(ifstream &inFile);
Node* block(ifstream &inFile);
Node* stats(ifstream &inFile);
Node* A(ifstream &inFile);
Node* M(ifstream &inFile);
Node* R(ifstream &inFile);
Node* mStat(ifstream &inFile);
Node* stat(ifstream &inFile);
Node* assign(ifstream &inFile);
Node* in(ifstream &inFile);
Node* out(ifstream &inFile);
Node* cond(ifstream &inFile); //if??
Node* loop(ifstream &inFile);
Node* assign(ifstream &inFile);
Node* RO(ifstream &inFile);
static void recGen(Node *root,ofstream &outfile);
//void generate(const Node *, Node *, FILE *);






#endif
