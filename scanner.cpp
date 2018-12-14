
#include "scanner.h"
#include "parser.h"

//header files

using namespace std; //f|| std use

Token exitToken(int,string,int);
string str2;



bool EOFflag = false;
bool cmmtFlagOn = false;
int row = 0;
int num = 1;

bool issymbol(char c){
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=')
	return true;
	else if (c == '>' || c == '<' || c == ':' || c == ','|| c == ';' || c== '.')
	return true;
	else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']')
	return true;
	else
	return false;
}
bool isComment(char c){
	//int flag = 0;
	if (c == '#')
		return true;


	return false;

}


bool isEOF(char c){

	if (c == EOF)
		return true;
	else
		return false;

}
bool iskeyword(string str2){


	if((str2 ==("begin")|| str2 ==("void")|| str2 ==("read")|| str2 ==("cond") || str2 ==("end")))
		return true;
	else if(str2 ==("end") || str2 ==("var") || str2 ==("print") || str2 ==("then") || str2 ==("iter"))
		return true;
	else if(str2 ==("return") || str2 ==("program") || str2 ==("let"))
		return true;
	else if((str2 ==(" begin ")|| str2 ==(" void ")|| str2 ==("read ")|| str2 ==(" cond ") || str2 ==(" end ")))
		return true;
	else if(str2 ==(" end ") || str2 ==(" var ") || str2 ==(" print ") || str2 ==(" then ") || str2 ==(" iter"))
		return true;
	else if(str2 ==(" return ") || str2 ==(" program ") || str2 ==(" let "))
		return true;
	else
	return false;
}

Token exitToken(int status, string str2,int num) {
	if (status == 1001) {
		if (isupper(str2.at(0))) {
			cout << "Scanner Error on line number: " << num << " " << endl;
			exit(0);
		}
	tok.tokenID = "varToken";
	tok.tokenInstant = str2;
	tok.lineNumber = num;
		if(iskeyword(str2)) {
			tok.tokenID = "keywordToken";
		}
	}
	else if(status == 1002)
	{
	tok.tokenID = "digitToken";
	tok.tokenInstant = str2;
	tok.lineNumber = num;

	}
	else if(status == 1003)
	{
	tok.tokenID = "symbolToken";
	tok.tokenInstant = str2;
	tok.lineNumber = num;
	}
	else if(status == 1004)
	{
		tok.tokenID = "eofToken";
		tok.tokenInstant = "EOF";
		tok.lineNumber = num;
	}
	else {
		cout << "*Scanner Error on line number: " << num << " " << endl;
		exit(-1);
	}



//	cout <<"\ntokenID: "<< tok.tokenID <<endl;
//	cout <<"Token created: tokenInstant: " << tok.tokenInstant <<endl;
//	cout <<"tokenLineNumber: " << tok.lineNumber << endl;
	return tok;

}


int getTableRow(int col, int row, int num, string str2){


	switch(col)
	{
		case 0: //letter
			//cout << "token table: " << TokenTable[row][col] <<endl;
			if (TokenTable[row][col] < 1000)
				row = TokenTable[row][col];
			else {
				exitToken(TokenTable[row][col], str2, num);
				//row = 0;
			}
			break;
		case 1: //digit
			//cout << "token table: " << TokenTable[row][col] <<endl;
			if (TokenTable[row][col] < 1000)
				row = TokenTable[row][col];
			else {
				exitToken(TokenTable[row][col], str2, num);
				//row = 0;

			}

			break;
		case 2: //symbol
			//col = col-1;]
//			cout << "col: " << col;
//			cout << "token table: " << TokenTable[row][col];
//			cout << "Row in side case2 " << row;
			//cout << "token table: " << TokenTable[row][col] <<endl;
			if (TokenTable[row][col] < 1000) {
				row = TokenTable[row][col];

				//cout << "row in side case2 " << row;
			}
			else {
				exitToken(TokenTable[row][col], str2, num);
				//row = 0;

			}
			break;
		case 3: //whitespace
			//col = col-1;
			//cout << "token table: " << TokenTable[row][col] <<endl;
			if (TokenTable[row][col] < 1000)
				row = TokenTable[row][col];
			else {
				exitToken(TokenTable[row][col], str2, num);
				//row = 0;

			}
			break;
		case 4: //endoffile

			if (TokenTable[row][col] < 1000)
				row = TokenTable[row][col];
			else {
				exitToken(TokenTable[row][col], str2, num);
				//row = 0;
			}
			break;
		case 5: //comments
			break;
		case -2:
			cout << "Scanner Error "<<endl;
			cout << "line number: " << num;
			exit(-1);
			break;
		default:
			col = -1;
			cout << "Scanner Error on line number: " << num << endl;
			exit(-1);

	}
	return row;
}
int getColumn(char c)
{
	if(isalpha(c)) {
		return 1;
	}
	else if(isdigit(c))
		return 2;
	else if(issymbol(c))
		return 3;
	else if(isspace(c))
		return 4;
	else if(isEOF(c))
		return 5;
	else if(isComment(c))
		return 6;
	else
		return -2;
}

Token scanner(ifstream &inFile) {

//variable for parsing the lines, strings, and characters of a file
	string str;
	char value;
	char lookAhead2 = '&';
	//cout << "********";

	if(!inFile.get(lookAhead2)){
		//cout << "**^^^*";
		exitToken(1004, "EOF",num);
		return tok;

	}
	inFile.unget();

	int x = 1;


	while (x==1) {
		inFile.get(value);
		str = value;
		//str2 = value;
		if ((value) == '\n') {
			num++;
		}
		char lookAhead = '&';
		char currChar = value;




		if(!inFile.get(lookAhead2)){
			lookAhead2=EOF;
		}
		inFile.unget();



		int col;
		//int row;

		string temp;

		bool flag = false;

		//while()
//		if (str.at(0) == '\n') {
//			str = " ";
//		}


		//--------comment logic--------------------------
		if ((isComment(str.at(0)))) {

			if ((cmmtFlagOn))
				cmmtFlagOn = false;
			else
				cmmtFlagOn = true;
			if(!cmmtFlagOn) {
				inFile.get(value);
				if ((value) == '\n') {
					num++;
				}
				str = value;
				if(!inFile.get(lookAhead2)){
					lookAhead2=EOF;
				}
				inFile.unget();


			}

			row = 0;
			col = 0;
			str2 = "";
		}



		//---------token logic-----------------------------
		if (!cmmtFlagOn) {

			if(!isspace(value))
				str2 = str2 + value;
			//cout << "string: " << str2 << endl;
			//cout << "lookAhead2: " << lookAhead2 << endl;
			col = (getColumn(value) - 1);
			row = getTableRow(col, row, num, str2);
//			cout << "col: " << col;
//			cout << "row: " << row << endl;
			if(TokenTable[row][col]>1000){
//				cout << "create token " << endl;
				exitToken(TokenTable[row][col], str2, num);

				row = 0;
				col = 0;
				str2 = "";
				return tok;

			}


		//----------look ahead is a symbol
			 if (issymbol(lookAhead2)) {
				//cout << "issymbol" << endl;
				flag = false;
				//cout << "string2 in symbol loop: " << str2 << endl;
				//cout << "lookahead: " << lookAhead <<endl;
				if (isspace(value)) {
					//cout << value;
					inFile.get(value);
					if ((value) == '\n') {
						num++;
					}
					//cout << value;
					str2 = value;
					exitToken(1003, str2, num);
					row = 0;
					col = 0;
					str2 = "";
					return tok;

				}
				//cout << "col :" << col <<endl;
				//cout << "row :" << row <<endl;
				//col = 2;
				exitToken(TokenTable[row][4], str2, num);
				str2 = "";
				row = 0;
				col = 0;
				return tok;

			}
			//----------if look ahead is EOF
			else if (isEOF(lookAhead2)) {
				//cout << "isEOF" << endl;

				//EOFflag=1;
				//cout << "lookahead: " << lookAhead << endl;
				//cout << value << endl;
				col = (getColumn(lookAhead2) - 1);
				//cout << col <<endl;
				 //cout << row <<endl;
				if (isspace(value)) {
					//cout << value;
					inFile.get(value);
					if ((value) == '\n') {
						num++;
					}
					str2 = value;
					exitToken(1004, str2, num);
					row = 0;
					col = 0;
					str2 = "";
					return tok;

				} else {
					//cout<<" before adding value "<<str2<<"  ";
					//str2 = str2 + value;
					row = getTableRow(col, row, num, str2);
					//cout<<" this row "<<row;
					exitToken(TokenTable[row][col], str2, num);


					row = 0;
					col = 0;
					str2 = "";
					return tok;
				}
			}

			//-------- look ahead not symbol or eof
			else
				{
				//cout<<"\n in  the lookahead not symbol or eof else section\n";
				//cout << row <<endl;
				if (row < 0)
				{
					cout << "Scanner error on line: " << num <<endl;
					exit(-1);
				}
				//cout << " " << col <<endl;
				col = (getColumn(value) - 1);
				row = getTableRow(col, row, num, str2);
			}
			if (row < 0)
			{
				cout << "Scanner error on line: " << num <<endl;
				exit(-1);
			}




			//---- create token---------------
			if(TokenTable[row][col]>1000){
				// << "create token " << endl;
				exitToken(TokenTable[row][col], str2, num);
				row = 0;
				col = 0;
				str2 = "";
				return tok;

			}



		}
		if(!inFile.get(lookAhead)){
			exitToken(1004, "EOF",num);
			//x=0;
			return tok;
		}
		inFile.unget();


	}
	return tok;

}




   // cout << tok.tokenInstant <<endl;

