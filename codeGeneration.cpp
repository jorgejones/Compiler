#include "codeGeneration.h"
#include <string>

int LabelCntr=0;                                     /* for unique labels */
int VarCntr=0;                                    /* for unique variables */
typedef enum {VAR, LABEL} nameType;
static char Name[20]; /* for creation of unique names */

vector<string> word;

string branch;
string branch2;

static char *newName(nameType what)
{ if (what==VAR)
        sprintf(Name,"V%d",VarCntr++);    /* generate a new label as V0, V1, etc */
    else
        sprintf(Name,"L%d",LabelCntr++);            /* new lables as L0, L1, etc */
    return(Name);
}
void error(string label,string token){
    cout<<" Error on <"<<label<<">: "<<label<<" and "<<token<<"\n";
    exit(1);
    return;
}


void codeGenerate(Node *root, fstream &outFile) {

    //outFile << "\tREAD\t%s\n" << root->token.tokenInstant;
    //cout << root->label << endl;
    //Token token = root -> token;
//    cout << root->child2->child2->child1->label << endl; //stat
//    int counter = 0;

    char label[20], label2[20], argR[20];
    if (root == NULL)
        return;

    else if (root->label== "program") {
		if(root->child1!=NULL)
        codeGenerate(root->child1, outFile);
		if(root->child2!=NULL)
        codeGenerate(root->child2, outFile);

		/*
        for(int i=0; i<VarCntr;i++){
            outFile<<"\nPOP";
        }
		*/

        outFile<<"\nSTOP";
		
		for(int i=0; i<VarCntr;i++){
            outFile<<"\nV"<<i<<"\t"<<0;
        }
		
		outFile<<"\n";
		
		return;

    }
	
	
	
	
    else if (root->label == "vars")
    {
        //cout << "VARS" <<endl;

		if(root->child3!=NULL){
			//cout << "\tchild1: " << root->child1->label <<"\n";
			//cout << "\tchild2: " << root->child2->label <<"\n";
			//cout << "\tchild3: " << root->child3->label <<"\n";
        outFile<<"\nLOAD\t"<<root->child3->token.tokenInstant;
		outFile<<"\nPUSH";
		outFile<<"\nSTACKW\t"<<0;
		}
        if(root->child4!=NULL)
            codeGenerate(root->child4, outFile);
		
		return;


    }else if (root->label == "block")
    {
        //cout << "BLOCK" <<endl;
		//cout << "\tchild1: " << root->child1->label <<"\n";
		//cout << "\tchild2: " << root->child2->label <<"\n";
		if(root->child1!=NULL)
			codeGenerate(root->child1, outFile);
		codeGenerate(root->child2, outFile);

		return;
		
    }
	
	
    else if (root->label == "stat")
    {
        //cout << "STAT";
		//cout << "\tchild1: " << root->child1->label <<"\n";
        codeGenerate(root->child1, outFile);
		
		return;

    }else if (root->label == "stats")
    {
        //cout << "STATS";
		//cout << "\tchild1: " << root->child1->label <<"\n";
		//cout << "\tchild2: " << root->child2->label <<"\n";
        codeGenerate(root->child1, outFile);
        codeGenerate(root->child2, outFile);
		
		return;

    }else if (root->label == "mStat")
    {
        //cout << "MSTAT";
		if(root->child1==NULL){
			//cout << " root-> child1 == NUll\n";
			//return;

		}
		else {
			//cout << "\tchild1: " << root->child1->label <<"\n";
			//cout << "\tchild2: " << root->child2->label <<"\n";
			codeGenerate(root->child1, outFile);
			codeGenerate(root->child2, outFile);
			//return;
		}
		return;

    }
   else if (root->label == "in")
    {
        //cout << "IN";
		
		
			//cout <<<<"in here now";
        //codeGenerate(root->child1, outFile);
		strcpy(argR,newName(VAR));
        outFile << "\nREAD\t"<<argR << endl;
        outFile << "\nLOAD\t"<< argR << endl;
		
		if(root->index[0]<0){		
			outFile<<"\nPUSH\t";
			outFile<<"\nSTACKW\t"<<0;
		}else{
			outFile<<"\nSTACKW\t"<<root->index[0];
		}
		//cout <<<<"made it to the bottom";
		
		
		return;

    }else if (root->label == "out")
    {
        //cout << "OUT";
		strcpy(argR,newName(VAR));
		string arg=argR;
		//if(root->child1!=NULL)
        codeGenerate(root->child1, outFile);
		//cout << "\tchild1: " << root->child1->label <<"\n";
		strcpy(argR,Name);
        outFile << "\nSTORE\t"<<argR << endl;
        outFile << "\nWRITE\t"<< argR << endl;
		
		return;

    }else if (root->label == "cond")
    {
		
        //cout << "COND";
		strcpy(label2,newName(LABEL));
		string mylabel=label2;
		
		
		//outFile<<"\n in IF:\t"<<label2<<":\tNOOP";
		
		if(root->child2!=NULL)
        codeGenerate(root->child2, outFile);
		strcpy(argR,newName(VAR));
		string arg=argR;
		
		
		
		codeGenerate(root->child1, outFile);
		//cout << "\t****child2: " << root->child1->label <<"\n";
		outFile<<"\nSTORE\t"<<arg;
		
		codeGenerate(root->child3, outFile);
		//cout << "\t***child3: " << root->child3->label <<"\n";
		outFile<<"\nSUB\t"<<arg;
		
		outFile<<branch<<mylabel;
		if(root->child2->child2!=NULL||root->child2->child1->token.tokenInstant == "="){
			if(root->child2->child2->token.tokenInstant!="=")
				outFile<<"\n"<<branch2<<mylabel;
		}
		codeGenerate(root->child4,outFile);
		word.clear();


		//outFile<<"\nin IF:\tBR"<<label2;
		outFile<<"\n"<<mylabel<<":\tNOOP";

		return;

    }
    else if (root->label == "assign") {
        //cout << "ASSIGN";
		

                    codeGenerate(root->child3, outFile);
					//codeGenerate(root->child1, outFile);
					strcpy(argR,newName(VAR));
                    outFile << "\nSTORE\t" <<argR<<endl;
                    outFile<<"\nSTACKW\t"<<root->index[0];
					
					
		return;			

}
  else if (root->label == "A") {
                    //cout << "A" <<endl;

					if(root->child3!=NULL){
						codeGenerate(root->child3, outFile);
						//cout << "\tchild3: " << root->child3->label <<"\n";
						strcpy(argR, newName(VAR));
						string arg=argR;
						outFile << "\nSTORE\t" << arg<<endl;
						
						
						codeGenerate(root->child1, outFile);
						//cout << "\tchild1: " << root->child1->label <<"\n";

							if (root->child2->token.tokenInstant == "+"){
								outFile << "\nADD\t" << arg << endl;
								//cout << "\tchild2: " << root->child2->label <<"\n";
							}
							else if (root->child2->token.tokenInstant == "-") {
								//cout << "\tchild2: " << root->child2->label <<"\n";
								outFile << "\nSUB\t" << arg << endl;
							}
							else {
								//cout << "error in expression node" << endl;
							}
						}
						else{
						codeGenerate(root->child1, outFile);
						//cout << "\tchild1: " << root->child1->label <<"\n";
						}
					
						
		return;			
}
else if (root->label=="M") {
        //cout << "M" <<endl;
		/*
		cout << "\tchild1: " << root->child1->label <<"\n";
		cout << "\tchild2: " << root->child2->label <<"\n";
		*/
				//cout<<"\nMarker\n";

		if(root->child1==NULL){
			codeGenerate(root->child3, outFile);
			//cout << "\tchild1: " << root->child1->label <<"\n";
		}
		else if(root->child1->label == "-"){

			//cout << "\tchild1: " << root->child1->label <<"\n";
			codeGenerate(root->child2, outFile);
			//cout<<" \nchild 2 label "<<root->child2->label;
			outFile << "\nMULT\t" << "-1" << endl;
		}
		return;

    }
     else if (root->label == "expr") {
        //cout << "EXPR" <<endl;

		if(root->child3!=NULL){
			codeGenerate(root->child3, outFile);
			//cout << "\tchild3: " << root->child3->label <<"\n";
			strcpy(argR, newName(VAR));
			string arg=argR;
			outFile << "\nSTORE\t"<<arg<<endl;
			if(root->child1!=NULL)
			codeGenerate(root->child1, outFile);
			//cout << "\tchild1: " << root->child1->label <<"\n";


			if (root->child2->token.tokenInstant == "*"){
				//cout << "\tchild2: " << root->child2->label <<"\n";
				outFile << "\nMULT\t" << arg << endl;

			}
			else if (root->child2->token.tokenInstant == "/") {
				//cout << "\tchild2: " << root->child2->label <<"\n";
				outFile << "\nDIV\t" << arg << endl;
			}
			else {
				//cout << "error in expression node" << endl;
			}
		}else{
			//cout << "\tchild1: " << root->child1->label <<"\n";
			codeGenerate(root->child1, outFile);
			//cout << "\tchild1: " << root->child1->label <<"\n";

		}
		
		return;
    } else if (root->label == "R") {
        //cout << "R" <<endl;

		if(root->child1!=NULL){
			codeGenerate(root->child1, outFile);
			//cout << "\tchild1: " << root->child1->label <<"\n";
		}
		else if(root->child2!=NULL){
			codeGenerate(root->child2, outFile);
			//cout << "\tchild2: " << root->child1->label <<"\n";
		}
		
		else if(root->child3!=NULL){
			//cout << "\tchild3: " << root->child3->label <<"\n";
			if(root->index[0]>=0){
				//cout << "\tchild3: " << root->child3->label <<"\n";
				outFile<<"\nSTACKR\t"<<root->index[0];
			}
			
		}
		
		else if(root->child4!=NULL){
			//cout << "\tchild4: " << root->child4->label <<"\n";
			outFile<<"\nLOAD\t"<<root->child4->token.tokenInstant;

			
		}else return;


			return;


    }else if (root->label == "loop")
    {
		
        //cout << "LOOP";
		strcpy(label,newName(LABEL));
		string mylabel=label;
		
		
		strcpy(label2,newName(LABEL));
		////cout<<" THE LABEL 2 is!"<<label2;
		string mylabel2=label2;
		
		outFile<<"\n"<<mylabel<<": NOOP\n";  //loop condition
		
		//expr
		//cout << "\tchild1: " << root->child1->label <<"\n";
        codeGenerate(root->child1, outFile);  //x
		strcpy(argR,newName(VAR));
		string arg=argR;
		
		outFile<<"\nSTORE\t"<<arg; //x
		
		
		//RO
		//cout << "\tchild2: " << root->child2->label <<"\n";
		codeGenerate(root->child2, outFile);  //operator

		//strcpy(argR,newName(VAR));
		//string arg2=argR;
		//outFile<<"\nin loop:\tSTORE\t"<<arg2; // store operator??
		//cout << "\tchild3: " << root->child2->label <<"\n";
		
		//expression
		codeGenerate(root->child3, outFile); // s
		outFile<<"\nSUB\t"<<arg;
		
		outFile<<branch<<mylabel2;
		if(root->child2->child2!=NULL||root->child2->child1->token.tokenInstant == "="){
			if(root->child2->child2->token.tokenInstant!="=")
				outFile<<"\n"<<branch2<<mylabel2;
		}
		
		codeGenerate(root->child4, outFile);
		
		/*
		for(int i=0;i<word.size();i++){
			outFile<<"\n "<<word[i]<<"\t"<<label2;
		}
		codeGenerate(root->child4,outFile);
		word.clear();

		*/
		
		
		outFile<<"\nBR\t"<<mylabel;
		outFile<<"\n"<<mylabel2<<":  NOOP\n";
		
		
		return;

    }else if (root->label== "RO")
	{
						string string1;
						string string2;	

						if(root->child2==NULL){
							if (root->child1->token.tokenInstant == "<"){
								branch="\nBRNEG\t";//+string(label2);
							}else if(root->child1->token.tokenInstant == ">"){
								branch="\nBRPOS\t";//+string(label2);	
							}else if(root->child1->token.tokenInstant == "="){
								branch= "\nBRPOS\t";//+string(label2);
								branch2="\nBRNEG\t";//+string(label2);

							}


							
						}else{
							if (root->child1->token.tokenInstant == "<"){
							
								branch= "\nBRNEG\t";//+string(label2);
								branch2="\nBRZERO\t";//+string(label2);
								
							}else if(root->child1->token.tokenInstant == ">"){
							
								branch= "\nBRPOS\t";//+string(label2);
								branch2="\nBRZERO\t";//+string(label2);
								
								/*
								string1= "\nBRPOS";
								string2="\nBRZERO";
								word.push_back(string1);
								word.push_back(string2);
								*/
							}else if(root->child1->token.tokenInstant == "="){
						
							branch="\nBRZERO\t";//+string(label2);
							
						
								/*
								//cout<<"in RO the label2 is"<<str;
								string1= "\nBRNEG";
								string2="\nBRPOS";
								word.push_back(string1);
								word.push_back(string2);
								*/

							}
						}

		return;
	}

 
}


/* generate assembler code for parse tree pointed by prog, with variables    */
/*   listed in st subtree                                                    */
static void generate(Node *root,Node *st,fstream &outFile)
{
    //nodeId =Label
    int i;
    if (root==NULL || root->label == "program" || root->child1 == NULL ||
        root->child1->label != "block")
        //cout << "something wrong with parse tree in generate" << endl;
    codeGenerate(root->child1->child2,outFile);     /* child1->child2 points to statList */
    //fprintf(outFile,"\tSTOP\n");
    outFile << "\tSTOP\n";
    while(st!=NULL)                  /* allocate storage for program variables */
    {
        //fprintf(outFile,"%s\tSPACE\n",st->token.tokenInstant);
        outFile<< st->token.tokenInstant << "\tSPACE\n" ;
        st=st->child1;
    }
    for (i=0;i<VarCntr; i++)      /* allocate space for temporary variables V* */
        //fprintf(outFile,"V%d\tSPACE\n",i);
        outFile<< "V" << i << "\tSPACE\n" ;
}
