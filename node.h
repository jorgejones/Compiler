#ifndef node_h
#define node_h

#include <string>
#include "scanner.h"
#include "token.h"

using namespace std;

struct Node {
    string label;
    Node *child1, *child2, *child3, *child4;
    Token token;
	vector<string> needpush;
	vector<int> index;
	vector<string> negation;
	
};

#endif