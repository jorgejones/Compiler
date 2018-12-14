
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
vector<string>global;  // for global scope
vector<string>temp;
bool scopeFlag = false;// for scope
bool varFlag = false;
int varCount = 0;
static int LabelCntr = 0;                                     /* for unique labels */
static int VarCntr = 0;                                    /* for unique variables */
char label[20], label2[20], argR[20];

typedef enum {VAR, LABEL} nameType;
static char Name[20];/* for creation of unique names */
Token tk;
Token tempTk;
int TokenTable [4][5]=

				//L    D    S  WS      eof

/*0B*/    {
			 {   2,    3,    1,    0, 1004},  //s1
/*1S*/       {1003, 1002, 1003, 1003, 1003},  //s2
/*2L*/       {   2,    2, 1001, 1001, 1001},  //s3
/*3D*/       {  -1,    3, 1002, 1002, 1002},  //s4

		};
	Token tok;


static char *newName(nameType what)
{
    if(what == VAR)
        sprintf(Name,"V%d",VarCntr++);    /* generate a new label as V0, V1, etc */
    else
        sprintf(Name,"L%d",LabelCntr++);            /* new labels as L0, L1, etc */
    return(Name);
}

        /* Parser function */

Node* parser(ifstream &inFile){
    std::ofstream file ("file.asm");
	Node *root = new Node;
	//root_node = NULL;
	root->label = "parser";
	//file << "my text here!" << std::endl;


	root = program(inFile);
    cout << "File parsed! " <<endl;
    file.close();

	return root;
}
/*      Program function
 *  void <vars><block>      */
Node* program(ifstream &inFile){
	Node *node = new Node;
	//node = NULL;
	node->label = "program";
	tk = scanner(inFile);
	//cout << tk.tokenInstant << " of program function " <<endl;
	if (tk.tokenInstant != "void")
		error(tk);
	else
	{
		tk = scanner(inFile);
		node -> child1 = vars(inFile);
		node -> child2 = block(inFile);
	}
	return node;
}


/*       Error function         */
void error(Token tk){
	cout << "\nParser error on line number: " << tk.lineNumber <<endl;
	cout << "Parser error on: " << tk.tokenInstant <<endl;
	exit(-1);
}


/*          Block function      */
/* BEGIN <vars> <stats> END */
Node* block(ifstream &inFile){
	//vector<int>blockCount;
    int blockVarCount= 0;
    //int index = 0
    //varCount = 0;

	Node *node = new Node;
	// node = NULL;
	node->label = "block";
	//cout << "Block function: " << tk.tokenInstant <<endl;
	if (tk.tokenInstant != "begin")
		error(tk);
	else
	{
	    scopeFlag = true;
	    if (varCount > 0)
	        varFlag = true;

        tk = scanner(inFile);
		node->child1 = vars(inFile);
		node->child2 = stats(inFile);
        //cout"Block function2: " << tk.tokenInstant <<endl;
		if (tk.tokenInstant != "end")
			error(tk);
		else {
		    //coutvarCount <<endl;
    	    blockVarCount = varCount;
		    while(blockVarCount > 0){

		        pop();
		        blockVarCount--;
		    }
		    //varCount = 0;
		    tk = scanner(inFile);
            ////cout"Block function3: " << tk.tokenInstant <<endl;
		    //remove(varCount);
            return node;
        }
	}
	return node;
}


/*          vars function           */
/* empty | let Identifier = Integer <vars> */
Node* vars(ifstream &inFile){
   // file.open("file.asm",fstream::out);
    //file();
	Node *node = new Node;
	// node = NULL;
	node->label = "vars";
	//tk = scanner(inFile);
	//cout << "vars function " << tk.tokenInstant <<endl;
	if(tk.tokenInstant == "let") {
		tk = scanner(inFile);
		//cout << "vars function2 " << tk.tokenInstant <<endl;
			if (tk.tokenID != "varToken") {
				error(tk);
			}
		else
			{
		    if(scopeFlag){
		        if(find(tk.tokenInstant) > 0 && find(tk.tokenInstant) < varCount) {
		            error(tk);
                }
		        else {
					node->needpush.push_back(tk.tokenInstant);
                    push(tk.tokenInstant);
                    varCount++;
                }
		    }
		    else
		        insert(tk.tokenInstant);

			node->child1 = new Node;
			node->child1->label = "Identifier";
			node->child1->token = tk;
			tk = scanner(inFile);
			//cout << "vars function3 " << tk.tokenInstant <<endl;
			if (tk.tokenInstant != "=") {

				error(tk);
			}

			else {
				node->child2 = new Node;
				node->child2->label = "Operator";
				node->child2->token = tk;
				tk = scanner(inFile);
				//cout << "vars function4 " << tk.tokenInstant <<endl;
				if (tk.tokenID != "digitToken")
					error(tk);
				else {
					node ->child3 = new Node;
					node ->child3->label = "digit Token";
					node->child3->token = tk;
					tk = scanner(inFile);
					//cout << "**var made**" <<endl;
                    //node ->child4 = new Node;
                    //node ->child4->label = "var";
                    node ->child4 = vars(inFile);
                    //if(scopeFlag)
                      //  varFlag = true;
                    //return node;
				}
			}

		}
	}

	return node;
}


/*          expr function       */
/* <A> / <expr> | <A> * <expr> | <A>           */
Node* expr(ifstream &inFile){

    //file();
	Node *node = new Node;
	// node = NULL;
	node->label = "expr";
    //tk = scanner(inFile);

    //cout << "expr function " << tk.tokenInstant <<endl;
	//cout<<" \nthe token instance in exp  is "<<tk.tokenInstant<<"\n";

    node->child1 = A(inFile);
	//cout<<" \nthe token instance in exp  is "<<tk.tokenInstant<<"\n";

    //tk = scanner(inFile);
   // cout << "expr function2 " << tk.tokenInstant <<endl;

    if((tk.tokenInstant == "/")||(tk.tokenInstant == "*")){
		node->child2 = new Node;
		node->child2->label = "Operator";
		node->child2->token = tk;
        /*  p4 code generation */

        tk = scanner(inFile);
        //cout << "expr function3 " << tk.tokenInstant <<endl;
        node->child3 = expr(inFile);
        //strcpy (argR,newName(VAR));
        //file << "\nSTORE\t";
        if ((node->child2->token.tokenInstant == "/")){
            //file << "EXPR\n";
            //file << "DIV " << tempTk.tokenInstant;
            //file();

        }
        else if((node->child2->token.tokenInstant == "*")) {
            //file << "MULT "<< tempTk.tokenInstant << endl;
            //file();
            return node;
        }
    }
    //file();
    return node;

}


/*          A function           */
/*  <M> + <A> | <M> - <A> | <M>   */
Node* A(ifstream &inFile) {
   // file.open("file.asm",fstream::out);
    //tk = scanner(inFile);
	Node *node = new Node;
	// node = NULL;
	node->label = "A";
    //cout << "A function " << tk.tokenInstant << endl;
	//cout<<" \nthe token instance in o  is "<<tk.tokenInstant<<"\n";
    node->child1=M(inFile);
	//cout<<" \nthe token instance in o  is "<<tk.tokenInstant<<"\n";

   // tk = scanner(inFile);
    //cout << "A function2 " << tk.tokenInstant << endl;

    if ((tk.tokenInstant == "+") || (tk.tokenInstant == "-"))
    {
		node->child2 = new Node;
		node->child2->label = "Operator";
    	node->child2->token = tk;
    	tempTk = node->child1->token;
    	//if(tempTk.tokenInstant == "+")
            //file << "ADD " << tempTk.tokenInstant << endl;
    	//else
    	    //file << "SUB " << tempTk.tokenInstant << endl;
        tk = scanner(inFile);
        //cout << "A function3 " << tk.tokenInstant << endl;
        node->child3 = A(inFile);
    }

    return node;
}


/*          M function
 *          - <M> |  <R>   */
Node* M(ifstream &inFile) {
	//cout<<" \nthe token instance in M is "<<tk.tokenInstant<<"\n";
    //file.open("file.asm",fstream::out);
	Node *node = new Node;
	// node = NULL;
	node->label = "M";
    //tk = scanner(inFile);
    //cout << "M function " << tk.tokenInstant << endl;
    if (tk.tokenInstant == "-"){
		node->child1 = new Node;
		node->child1->label = "negative";
    	node->child1->token = tk;
        tk = scanner(inFile);
        //file << "READ\t-1\t"<<endl;
        //cout << "M function2 " << tk.tokenInstant << endl;
        node->child2= M(inFile);
        //file << "MULT " << node->child2 << endl;
    }
    else
        node->child1=R(inFile);
    return node;
}


/*          R function
 *       ( <expr> ) | Identifier | Integer */
Node* R(ifstream &inFile){
    //file.open("file.asm",fstream::out);
	Node *node = new Node;
	node->label = "R";
    //tk = scanner(inFile);
    //cout << "R function " << tk.tokenInstant <<endl;
	//cout<<" \nthe token instance in R is "<<tk.tokenInstant<<"\n";
	
    if (tk.tokenInstant == "(") {
        tk = scanner(inFile);
        node->child1 = expr(inFile);
        //tk = scanner(inFile);
        //cout << "R function2: " << tk.tokenInstant <<endl;

        if(tk.tokenInstant != ")")
            error(tk);
        else {
            //cout << "R function3: " << tk.tokenInstant <<endl;
            tk = scanner(inFile);
            //node->child2->token = tk;
            return node;
        }
    }
    else if (tk.tokenID == "varToken")
    {
        if(find(tk.tokenInstant) < 1)
            verify(tk.tokenInstant);
       // verify(tk.tokenInstant);
		node ->child3 = new Node;
		node ->child3->label = "Identifier";
    	node->child3->token = tk;
		int n=find(tk.tokenInstant);
		node->index.push_back(n);
        tk = scanner(inFile);
        //file << "MULT " << tempTk.tokenInstant << endl;
        //cout << "R function3 " << tk.tokenInstant <<endl;
        return node;
    }
    else if (tk.tokenID == "digitToken") {
		node ->child4 = new Node;
		node ->child4->label = "Digit";
		node->child4->token = tk;
        tk = scanner(inFile);
        //cout << "R function4 " << tk.tokenInstant <<endl;
        return node;
    }
	return node;
}


/*          stats function
 *           <stat>  <mStat>        */
Node* stats(ifstream &inFile){
	Node *node = new Node;
	// node = NULL;
	node->label = "stats";
	//cout << "stats function " << tk.tokenInstant <<endl;
	node->child1 = stat(inFile);
	node->child2 = mStat(inFile);
	return node;

}


/*      mStat Function
 *            empty |  <stat>  <mStat> */
Node* mStat(ifstream &inFile){
	Node *node = new Node;
	// node = NULL;
	node->label = "mStat";
	//tk = scanner(inFile);

	//cout << "mStat function " << tk.tokenInstant <<endl;
//  //  if(tk.tokenInstant == "end")
//    //    return node;
    if((tk.tokenInstant == "read")||(tk.tokenInstant == "print")||(tk.tokenInstant == "cond")
	||(tk.tokenInstant == "iter")||(tk.tokenID == "varToken")||(tk.tokenInstant == "begin"))
	{

	    node->child1 = stat(inFile);
        node->child2 = mStat(inFile);
        return node;
	}

	return node;


}


/*          stat function
 *  <in> | <out> | <block> | <if> | <loop> | <assign> */
Node* stat(ifstream &inFile){
	Node *node = new Node;
	// node = NULL;
	node->label = "stat";
	//cout << "stat function " << tk.tokenInstant <<endl;

	if(tk.tokenInstant == "read")
		node->child1 = in(inFile);
	else if(tk.tokenInstant == "print")
		node->child1 = out(inFile);
	else if(tk.tokenInstant == "begin")
		node-> child1 = block(inFile);
	else if(tk.tokenInstant == "cond")
		node->child1 = cond(inFile);
	else if(tk.tokenInstant == "iter")
		node->child1 = loop(inFile);
	else if(tk.tokenID == "varToken")
		node->child1 = assign(inFile);
	else
	    error(tk);

	return node;
}


/*          in function
 *          read(identifier): */
Node* in(ifstream &inFile){
	Node *node = new Node;
	// node = NULL;
	node->label = "in";
	//tk = scanner(inFile);
	//cout << "in function " << tk.tokenInstant <<endl;
	if(tk.tokenInstant != "read")
		error(tk);
	else{
		tk = scanner(inFile);
        //cout << "in function2 " << tk.tokenInstant <<endl;
		if(tk.tokenInstant != "(")
			error(tk);
		else{
			tk = scanner(inFile);
            //cout << "in function3 " << tk.tokenInstant <<endl;
			if(tk.tokenID != "varToken")
				error(tk);
			else{
				int n=find(tk.tokenInstant);
				//cout<<" n right now is "<<n<<endl;
				node->index.push_back(n);
                if(n < 0)
                    verify(tk.tokenInstant);
			    //verify(tk.tokenInstant);
				node->child1 = new Node;
				node->child1->label = "Identifier";
				node->child1->token = tk;
				/*  p4 code generation */
                //file.open("file.asm",fstream::out);
				//file << "READ " << tk.tokenInstant << endl;




				tk = scanner(inFile);
                //cout << "in function4 " << tk.tokenInstant <<endl;
				if(tk.tokenInstant != ")")
					error(tk);
				else{
                    tk = scanner(inFile);
                    //cout << "in function5 " << tk.tokenInstant <<endl;
					if(tk.tokenInstant != ":")
						error(tk);
					else{

                        //cout << "**Expression read**" << endl;
                        tk = scanner(inFile);

						return node;
					}
				}
			}
		}
	}
	return node;

}


/*          out function
 *          print( <expr> ): */
Node* out(ifstream &inFile){
    //file.open("file.asm",fstream::out);
	

	Node *node = new Node;
	// node = NULL;
	node->label = "out";
	//cout << "out function " << tk.tokenInstant <<endl;
	if(tk.tokenInstant != "print")
		error(tk);
	else{
		tk = scanner(inFile);
		//cout << "out function2 " << tk.tokenInstant <<endl;
		if(tk.tokenInstant != "(")
			error(tk);
		else{
			tk = scanner(inFile);
			//cout<<" \nthe token instance in s  is "<<tk.tokenInstant<<"\n";
			node->child1 = expr(inFile);
			//cout<<" \nthe token instance in a  is "<<tk.tokenInstant<<"\n";

			//cout << "out function3 " << tk.tokenInstant <<endl;
			if(tk.tokenInstant != ")")
				error(tk);
			else{
				tk = scanner(inFile);
				//cout << "out function4 " << tk.tokenInstant <<endl;

				if(tk.tokenInstant != ":")
					error(tk);
				else{
                    //cout << "**Expression printed**"  <<endl;
                    tk = scanner(inFile);
					return node;
				}
			}
		}
	}
	return node;
}


/*          if/cond function
 *          cond( <expr> <RO> <expr> ) <stat> */
Node* cond(ifstream &inFile){
    //file.open("file.asm",fstream::out);
	Node *node = new Node;
	// node = NULL;
	node->label = "cond";
	//tk = scanner(inFile);
	//cout << "cond function " << tk.tokenInstant <<endl;
	if(tk.tokenInstant != "cond")
		error(tk);
	else{
		tk = scanner(inFile);
        //cout << "cond function2 " << tk.tokenInstant <<endl;
		if(tk.tokenInstant != "(")
			error(tk);
		else{
            tk = scanner(inFile);
            //cout << "cond function3 " << tk.tokenInstant <<endl;
			node->child1 = expr(inFile);
            //cout << "cond function4 " << tk.tokenInstant <<endl;
			node->child2 = RO(inFile);
            //cout << "cond function5 " << tk.tokenInstant <<endl;
			node->child3 =expr(inFile);
            //file << "\nSTORE\t" <<node->child3->token.tokenInstant;
            if(node->child2->token.tokenInstant == "<") {
                //file << "\nBRPOS\tnode->child2->token.tokenInstant\n";
                //file << "\nBRZERO\tnode->child2->token.tokenInstant\n";
            }
            if(node->child2->token.tokenInstant == "<=") {
                //file << "\nBRPOS\tnode->child2->token.tokenInstant\n";
                //file << "\nBRZERO\tnode->child2->token.tokenInstant\n";
            }
			//tk = scanner(inFile);
            //cout << "cond function5 " << tk.tokenInstant <<endl;
			if(tk.tokenInstant != ")")
				error(tk);
			else{
                //cout << "** condition passed **"<<endl;
                tk = scanner(inFile);
                node->child4 = stat(inFile);
				return node;
			}
		}
	}
	return node;
}


/*          loop function
 *          iter( <expr> <RO> <expr> ) <stat> */
Node* loop(ifstream &inFile){
	Node *node = new Node;
	// node = NULL;
	node->label = "loop";
	//tk = scanner(inFile);
	//cout << "loop function " << tk.tokenInstant <<endl;
	if(tk.tokenInstant != "iter")
		error(tk);
	else{
		tk = scanner(inFile);
        //cout << "loop function2 " << tk.tokenInstant <<endl;
		if(tk.tokenInstant != "(")
			error(tk);
		else{
            tk = scanner(inFile);
            //cout << "loop function3 " << tk.tokenInstant <<endl;
			node->child1 = expr(inFile);
            //cout << "loop function4 " << tk.tokenInstant <<endl;
			node->child2 = RO(inFile);
            //cout << "loop function5 " << tk.tokenInstant <<endl;
			node->child3 = expr(inFile);
            ////cout << "loop function6 " << tk.tokenInstant <<endl;
			//tk = scanner(inFile);
            //cout << "loop function7 " << tk.tokenInstant <<endl;
			if(tk.tokenInstant != ")")
				error(tk);
			else{
                tk = scanner(inFile);
                //cout << "**loop condition** " << tk.tokenInstant <<endl;
				node->child4 = stat(inFile);
				return node;
			}
		}
	}
	return node;
}


 /*         assign function         */
 /*         Identifier = <expr> : */
Node* assign(ifstream &inFile){
     //file("file.asm",fstream::out);
	 Node *node = new Node;
	 // node = NULL;
	 node->label = "assign";
	//tk = scanner(inFile);
	//cout << "assign function " << tk.tokenInstant <<endl;
	if(tk.tokenID != "varToken")
		error(tk);
	else{
		int n=find(tk.tokenInstant);
		node->index.push_back(n);
	    if( n< 1)
            verify(tk.tokenInstant);
		node->child1 = new Node;
		node->child1->label = "Identifier";
		node->child1->token = tk;
        //cout << "\nSTORE\t" <<node->child1->token.tokenInstant;
        //file << "\nSTORE\t" <<node->child1->token.tokenInstant;
		tk = scanner(inFile);
		//cout << "assign function2 " << tk.tokenInstant <<endl;
		if(tk.tokenInstant != "=")
			error(tk);
		else
			{
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
            tk = scanner(inFile);
            //out << "assign function3 " << tk.tokenInstant <<endl;
			node->child3 = expr(inFile);
			//tk = scanner(inFile);
			////cout << "assign function3 " << tk.tokenInstant <<endl;
			if(tk.tokenInstant != ":")
				error(tk);
			else{
                //cout << "**Expression Assigned**" <<endl;
                tk = scanner(inFile);
				//return node;
			}

		}
	}
	//file();
	return node;
}


/*             RO function
*    < | <  = | >  | >  = | =  =  |   =   */
Node* RO(ifstream &inFile){
    //file("file.asm",fstream::out);
    //file();
	Node *node = new Node;
	// node = NULL;
	node->label = "RO";
	////cout << "RO " << tk.tokenInstant <<endl;
	//tk = scanner(inFile);
    //cout << "RO " << tk.tokenInstant <<endl;
	if(tk.tokenInstant == "<")
	{
		node->child1 = new Node;
		node->child1->label = "Operator";
		node->child1->token = tk;

		tk = scanner(inFile);
        //cout << "RO function 2 " << tk.tokenInstant <<endl;
		if(tk.tokenInstant == "=")
		{
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
            tk = scanner(inFile);
            //cout << "RO function 3 " << tk.tokenInstant <<endl;
            return node;
        }
		else
			return node;
	}
	else if(tk.tokenInstant == ">")
	{

		node->child1 = new Node;
		node->child1->label = "Operator";
		node->child1->token = tk;
		tk = scanner(inFile);
        //cout << "RO function 2: " << tk.tokenInstant <<endl;
		if(tk.tokenInstant == "=") {
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
		    tk = scanner(inFile);
            //cout << "RO function 3: " << tk.tokenInstant <<endl;
            return node;
        }
		else
			return node;
	}
	else if(tk.tokenInstant == "=")
	{
		node->child1 = new Node;
		node->child1->label = "Operator";
		node->child1->token = tk;
		tk = scanner(inFile);
        //cout << "RO function 2: " << tk.tokenInstant <<endl;
		if(tk.tokenInstant == "="){
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
		    tk = scanner(inFile);
            //cout << "RO function 3: " << tk.tokenInstant <<endl;
		    return node;
		}
		else
			return node;
	}
	else
		error(tk);
	return node;
}
/* function to check if a variable has been previously defined globally, if it has not, defines it */
bool insert(string str){
    //if(scopeFlag == true)
        //return true;

    if(!scopeFlag){
    if(find (global.begin(),global.end(),str)!= global.end()){
        //cout << "Global scoping issues with declaring var: " << str <<endl;
        error(tk);
    }
    else {
        global.insert(global.begin(), str);
        //cout << "Global variable " << str << ": declared" <<endl;
    }
    return true;
    }
    else{
        push(str);
        varCount++;
        return true;
    }
}


bool verify(string str){

    if(find (global.begin(),global.end(),str)!= global.end()){
        //cout << "Global variable " << str << ": implemented" <<endl;
        return true;
    }
    else {
        //cout << "verify";
		for (std::vector<string>::const_iterator i = global.begin(); i != global.end(); ++i)
			std::cout << "global vector: " << *i << ' ';
        //cout << "Global scoping issues with using var: " << str << endl;
        error(tk);

    }
    return true;

}

void push(string str){
    global.push_back(str);
}

void pop(){
	//for (std::vector<string>::const_iterator i = global.begin(); i != global.end(); ++i)
	//	std::cout << "global vector: " << *i << ' ';
    global.pop_back();
}
int find(string str){

    int count = 0;
	string temp[count];
    //cout << "string in find " <<  str <<endl;
    vector<string>::iterator it;
    it =(find (global.begin(),global.end(),str));
    if(it != global.end()){
        while (global.back() != str)
        {
        	if(global.back() != str) {
				temp[count] = global.back();
			}

            pop();
            count++;
        }
		for(int i = 0; i < count; i++){
			global.push_back(temp[0]);
		}

        //cout << count << endl;
        return count;
    }
    else{
        //cout << "Couldnt find " << str << endl;
        return -1;
    }
}

//static void recGen(Node *stats,fstream &outFile)
//{
//    char label[20], label2[20], argR[20];
//    if (stats==NULL)
//        return;
//    else if(stats->label == "in")
//            outFile<<"\tREAD\t%s\n"<<stats->token.tokenInstant;
//
////        case assign:
////            recGen(stats->child1,out);           /* evaluate rhs */
////            fprintf(out,"\tSTORE\t%s\n",stats->tokenP->str);
////            break;
////        case expr:
////            recGen(stats->child2,out);     /* first evaluate rhs */
////            strcpy(argR,newName(VAR));
////            fprintf(out,"\tSTORE\t%s\n",argR);
////            recGen(stats->child1,out);       /* result is in ACC */
////            if (stats->child2->token->tokenInstance == "+")
////                fprintf(out,"\tADD\t%s\n",argR);
////            else if (stats->tokenP->tokenId==MINUStk)
////                fprintf(out,"\tSUB\t%s\n",argR);
////            else error("Invalid tokenId in expression node",
////                       stats->tokenP->str,stats->tokenP->line);
////            break;
////
////        case cond:
////            recGen(stats->child2,out);
////            strcpy(argR,newName(VAR));
////            fprintf(out,"\tSTORE\t%s\n",argR);
////            recGen(stats->child1,out);
////            fprintf(out,"\tSUB\t%s\n",argR);
////            strcpy(label,newName(LABEL));
////            switch (stats->tokenP->tokenId)
////            { case LESStk:  fprintf(out,"\tBRPOS\t%s\n",label);
////                    fprintf(out,"\tBRZERO\t%s\n",label);
////                    break;
////                case LESSEQtk:
///                 fprintf(out,"\tBRPOS\t%s\n",label);
////                    break;
////                case GREATERtk: fprintf(out,"\tBRNEG\t%s\n",label);
////                    fprintf(out,"\tBRZERO\t%s\n",label);
////                    break;
////                case GREATEREQtk: fprintf(out,"\tBRNEG\t%s\n",label);
////                    break;
////                case EQUALtk: fprintf(out,"\tBRNONZ\t%s\n",label);
////                    break;
////                default: fprintf(out,"\tBRZERO\t%s\n",label);
////            }
////            break;
////        case ifNode:
////            recGen(stats->child1,out);              /* condition */
////            strcpy(label,Name);           /* memorize jump label */
////            recGen(stats->child2,out);   /* dependent statements */
////            fprintf(out,"%s\tNOOP\n",label);
//////            break;
////        default:
////            recGen(stats->child1,out);
////            recGen(stats->child2,out);
////            recGen(stats->child3,out);
  //  else return;
   // }

///* generate assembler code for parse tree pointed by prog, with variables    */
///*   listed in st subtree                                                    */
////void generate(const Node *prog, Node *st, FILE *out)
////{
////    int i;
////    if (prog == NULL || prog->token=programNode || prog->child1==NULL ||
////        prog->child1->token=programBlockNode)
////        cout << "something wrong with parse tree in generate" << endl;
////    recGen(prog->child1->child2,out);     /* child1->child2 points to statList */
////    fprintf(out,"\tSTOP\n");
////    while(st!=NULL)                  /* allocate storage for program variables */
////    {
////        fprintf(out,"%s\tSPACE\n",st->tokenP->str);
////        st=st->child1;
////    }
////    for (i=0;i<VarCntr; i++)      /* allocate space for temporary variables V* */
////        fprintf(out,"V%d\tSPACE\n",i);
////}
