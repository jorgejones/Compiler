#ifndef TOKEN_H
#define TOKEN_H
#include <string>
using namespace std;


struct Token
{
string tokenID;
string tokenInstant;
int lineNumber;
};
extern Token tok;


#endif 
