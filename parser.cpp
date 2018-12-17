
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


vector<int> blockv;
int blockvindex=0;

        /* Parser function */

Node* parser(ifstream &inFile){
	
    std::ofstream file ("file.asm");
	Node *root = new Node;
	//root_node = NULL;
	root->label = "parser";
	//file << "my text here!" << std::endl;


	root=program(inFile);
    cout << "File parsed! " <<endl;
    file.close();

	return root;
}
/*      Program function
 *  void <vars><block>      */
 
 
 
 
 
 
 
Node* program(ifstream &inFile){
	blockv.push_back(0);
	
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
	//outFile<<"\nSTOP";
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
    int blockVarCount= 0;
    

	Node *node = new Node;
	node->label = "block";
	
	
	if (tk.tokenInstant != "begin")
		error(tk);
	else
	{
		
		
		blockvindex++;
		blockv.push_back(0);
		
		
		/*
	    scopeFlag = true;
	    if (varCount > 0)
	        varFlag = true;
		*/
		
		
		
		
        tk = scanner(inFile);
		node->child1 = vars(inFile);
		
		node->child2 = stats(inFile);
		
        //cout"Block function2: " << tk.tokenInstant <<endl;
		
		if (tk.tokenInstant != "end")
			error(tk);
		else {
			tk = scanner(inFile);
			
			for(int i=0;i<blockv[blockvindex];i++){
					pop();
					blockv[blockvindex]--;
			}
			
			blockvindex--;
			
			/*
			//pop number of count
		    //coutvarCount <<endl;
    	    blockVarCount = varCount;
		    while(blockVarCount > 0){

		        pop();
		        blockVarCount--;
		    }
		    //varCount = 0;
		    
			
			*/
			
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
	node->label = "vars";
	   
	   
	if(tk.tokenInstant == "let") {
		
		tk = scanner(inFile);
		//cout << "vars function2 " << tk.tokenInstant <<endl;
			if (tk.tokenID != "varToken") {
				error(tk);
			}
		else
			{
				//needs scoping
		        if(find(tk.tokenInstant) >= 0) {
					insert(tk.tokenInstant);
                }
		        else {
					//node->needpush.push_back(tk.tokenInstant);
                    push(tk.tokenInstant);
                    blockv[blockvindex]++;
                }
					
			node->child1 = new Node;
			node->child1->label = "Identifier";
			node->child1->token = tk;
			tk = scanner(inFile);
			//cout << "vars function3 " << tk.tokenInstant <<endl;
			if (tk.tokenInstant != "=") {

				error(tk);
			}

			else {
				node ->child2 = new Node;
				node ->child2->label = "Operator";
				node->child2->token = tk;
				tk = scanner(inFile);
				if (tk.tokenID != "digitToken")
					error(tk);
				else {
					node ->child3 = new Node;
					node ->child3->label = "digit Token";
					node->child3->token = tk;
					tk = scanner(inFile);
                    node ->child4 = vars(inFile);
                    
				}
			}

		}
	}

	return node;
}


/*          expr function       */
/* <A> / <expr> | <A> * <expr> | <A>           */
Node* expr(ifstream &inFile){

	Node *node = new Node;
	node->label = "expr";
    //tk = scanner(inFile);

    //cout << "expr function " << tk.tokenInstant <<endl;
	//cout<<" \nthe token instance in exp  is "<<tk.tokenInstant<<"\n";

    node->child1 = A(inFile);

    if((tk.tokenInstant == "/")||(tk.tokenInstant == "*")){
		node->child2 = new Node;
		node->child2->label = "Operator";
		node->child2->token = tk;
		
        tk = scanner(inFile);
        node->child3 = expr(inFile);
    }
    return node;

}


/*          A function           */
/*  <M> + <A> | <M> - <A> | <M>   */
Node* A(ifstream &inFile) {
   
	Node *node = new Node;
	node->label = "A";
    
    node->child1=M(inFile);
	

    if ((tk.tokenInstant == "+") || (tk.tokenInstant == "-"))
    {
		node->child2 = new Node;
		node->child2->label = "Operator";
    	node->child2->token = tk;
    	tempTk = node->child1->token;
        tk = scanner(inFile);
		
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
		node->child1->label = "-";
    	node->child1->token = tk;
        tk = scanner(inFile);

        node->child2= M(inFile);
    }
    else
        node->child3=R(inFile);
    return node;
}


/*          R function
 *       ( <expr> ) | Identifier | Integer */
Node* R(ifstream &inFile){
	Node *node = new Node;
	node->label = "R";
   
	
    if (tk.tokenInstant == "(") {
        tk = scanner(inFile);
        node->child1 = expr(inFile);

        if(tk.tokenInstant != ")")
            error(tk);
        else {
            tk = scanner(inFile);
            return node;
        }
    }
    else if (tk.tokenID == "varToken")
    {
        if(find(tk.tokenInstant) < 1)	
            verify(tk.tokenInstant);
		
		node ->child3 = new Node;
		node ->child3->label = "Identifier";
    	node->child3->token = tk;
		
		int n=find(tk.tokenInstant);
		if(node->index.empty())
			node->index.push_back(n);	
		else
			node->index[0]=n;
		
        tk = scanner(inFile);
        
        return node;
    }
    else if (tk.tokenID == "digitToken") {
		node ->child4 = new Node;
		node ->child4->label = "Digit";
		node->child4->token = tk;
        tk = scanner(inFile);
        return node;
    }
	return node;
}


/*          stats function
 *           <stat>  <mStat>        */
Node* stats(ifstream &inFile){
	
	Node *node = new Node;
	node->label = "stats";
	node->child1 = stat(inFile);
	
	node->child2 = mStat(inFile);
	return node;

}


/*      mStat Function
 *            empty |  <stat>  <mStat> */
Node* mStat(ifstream &inFile){
	Node *node = new Node;
	node->label = "mStat";

	
    if((tk.tokenInstant == "read")||(tk.tokenInstant == "print")||(tk.tokenInstant == "cond")
	||(tk.tokenInstant == "iter")||(tk.tokenID == "varToken")||(tk.tokenInstant == "begin"))
	{

	    node->child1 = stat(inFile);
        node->child2 = mStat(inFile);
	}

	return node;

}





/*          stat function
 *  <in> | <out> | <block> | <if> | <loop> | <assign> */
Node* stat(ifstream &inFile){
	Node *node = new Node;
	node->label = "stat";

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
	node->label = "in";
	
	
	
	if(tk.tokenInstant != "read")
		error(tk);
	else{
		tk = scanner(inFile);
		if(tk.tokenInstant != "(")
			error(tk);
		else{
			tk = scanner(inFile);
			if(tk.tokenID != "varToken")
				error(tk);
			else{
				int n=find(tk.tokenInstant);
				//cout<<"tk in  in. right now is "<<tk.tokenInstant<<" "<<n<<endl;
				if(node->index.empty())
					node->index.push_back(n);	
				else
					node->index[0]=n;
				
				//cout<<"!!!!!!!"<<node->index[0];
                if(n > 0)
                    verify(tk.tokenInstant);
				
			    //verify(tk.tokenInstant);
				node->child1 = new Node;
				node->child1->label = "Identifier";
				node->child1->token = tk;

				tk = scanner(inFile);
				if(tk.tokenInstant != ")")
					error(tk);
				else{
                    tk = scanner(inFile);
					if(tk.tokenInstant != ":")
						error(tk);
					else{
						
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
	if(tk.tokenInstant != "print")
		error(tk);
	else{
		tk = scanner(inFile);
		
		if(tk.tokenInstant != "(")
			error(tk);
		else{
			tk = scanner(inFile);
			node->child1 = expr(inFile);

			if(tk.tokenInstant != ")")
				error(tk);
			else{
				tk = scanner(inFile);

				if(tk.tokenInstant != ":")
					error(tk);
				else{
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
	node->label = "cond";
	
	if(tk.tokenInstant != "cond")
		error(tk);
	else{
		
		tk = scanner(inFile);
		
		if(tk.tokenInstant != "(")
			error(tk);
		else{
            tk = scanner(inFile);
			node->child1 = expr(inFile);
			
			node->child2 = RO(inFile);
			
			node->child3 =expr(inFile);
		
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
	node->label = "loop";
	
	if(tk.tokenInstant != "iter")
		error(tk);
	else{
		tk = scanner(inFile);
		
		if(tk.tokenInstant != "(")
			error(tk);
		else{
            tk = scanner(inFile);
			
			node->child1 = expr(inFile);
			
			node->child2 = RO(inFile);
			
			node->child3 = expr(inFile);
			
			if(tk.tokenInstant != ")")
				error(tk);
			else{
                tk = scanner(inFile);
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
	 Node *node = new Node;
	 node->label = "assign";
	 
	 
	 
	if(tk.tokenID != "varToken")
		error(tk);
	else{
		
		int n=find(tk.tokenInstant);
		if(node->index.empty())
			node->index.push_back(n);	
		else
			node->index[0]=n;
		
		//cout<<"\n  tk  "<<tk.tokenInstant<<" n is "<<n<<"  global at n is "<<global[n]<<endl;
		/*
		for(int i=0;i<global.size();i++){
			cout<<"\n"<<global[i];
		} */
		
		//cout<<"\t\tin assign n is "<<n<<endl;
		//cout<<"\n\n\n\n Marker\n\n\n\n";
		
	    if( n< 1)
          verify(tk.tokenInstant);
		
		node->child1 = new Node;
		node->child1->label = "Identifier";
		node->child1->token = tk;
       
		tk = scanner(inFile);
		
		if(tk.tokenInstant != "=")
			error(tk);
		else
			{
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
            tk = scanner(inFile);
			
			node->child3 = expr(inFile);
			
			
			if(tk.tokenInstant != ":")
				error(tk);
			else{
                tk = scanner(inFile);
			}

		}
	}
	//file();
	return node;
}




/*             RO function
*    < | <  = | >  | >  = | =  =  |   =   */
Node* RO(ifstream &inFile){
    
	Node *node = new Node;
	// node = NULL;
	node->label = "RO";
	node->child1=NULL;
	node->child2=NULL;

	if(tk.tokenInstant == "<")
	{
		node->child1 = new Node;
		node->child1->label = "Operator";
		node->child1->token = tk;

		tk = scanner(inFile);
		
		if(tk.tokenInstant == "=")
		{
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
            tk = scanner(inFile);
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
		
		if(tk.tokenInstant == "=") {
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
		    tk = scanner(inFile);
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
		
		if(tk.tokenInstant == "="){
			node->child2 = new Node;
			node->child2->label = "Operator";
			node->child2->token = tk;
		    tk = scanner(inFile);
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
    
	int n=find(str);
		
    if(n<blockv[blockvindex]){
		cout<<" This variable has already been declared in this scope";
		exit(1);
		return false;
    }
    else{
        global.push_back(str);
        blockv[blockvindex]++;
        return true;
    }
}







bool verify(string str){

    if(find (global.begin(),global.end(),str)!= global.end()){
        return true;
    }
    else {
		cout<<"variable not defined in this scope";
		for (std::vector<string>::const_iterator i = global.begin(); i != global.end(); ++i)
			//std::cout << "global vector: " << *i << ' ';
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
		int flag=0;
		int stackindex=0;
		int mycount=0;
        
		/*
            for(int i = 0; i < count; i++){
                global.push_back(temp[0]);
            } */
			
			for(int index=global.size()-1;index>=0;index--){
				if(global[index]==str){
					stackindex=index;
					flag=1;
				}
				mycount++;
				
			}

         if(flag==1){
			//cout<<"\n the index is "<<stackindex;

			//cout<<"\n the count is "<<mycount;
            return (mycount-1)-stackindex;
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
