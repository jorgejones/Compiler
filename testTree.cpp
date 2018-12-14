#include <stdio.h>
#include "token.h"
#include "testTree.h"

void printTree(Node *root, int depth) {
    if (root == NULL) {
        return;
    }
    else {
        for (int i = 0; i < depth; i++) {
            cout << "  ";
        }
        depth++;
        cout << root->label;
        Token token = root -> token;
        if (token.tokenInstant != "") {
            cout << "- tk: " <<token.tokenID << ", label: " << token.tokenInstant << " ";
        }
        
        cout <<endl;
        if(root->child1 != NULL){
            printTree(root->child1, depth);
        }
        if(root->child2 != NULL){
            printTree(root->child2, depth);
        }
        if(root->child3 != NULL){
            printTree(root->child3, depth);
        }
        if(root->child4 != NULL){
            printTree(root->child4, depth);
        }
    }
}