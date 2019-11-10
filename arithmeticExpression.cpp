#include "arithmeticExpression.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int arithmeticExpression::priority(char op){
    int priority = 0;
    if(op == '('){
        priority =  3;
    }
    else if(op == '*' || op == '/'){
        priority = 2;
    }
    else if(op == '+' || op == '-'){
        priority = 1;
    }
    return priority;
}

string arithmeticExpression::infix_to_postfix(){
    stack<char> s;
    ostringstream oss;
    char c;
    for(unsigned i = 0; i< infixExpression.size();++i){
        c = infixExpression.at(i);
        if(c == ' '){
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){
                    oss << s.top();
                    s.pop();
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    while(!s.empty()){
        oss << s.top();
        s.pop();
    }
    return oss.str();
}

void arithmeticExpression::visualizeTree(const string &outputFilename){
    ofstream outFS(outputFilename.c_str());
    if(!outFS.is_open()){
        cout<<"Error opening "<< outputFilename<<endl;
        return;
    }
    outFS<<"digraph G {"<<endl;
    visualizeTree(outFS,root);
    outFS<<"}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0,outputFilename.size()-4)+".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}

void arithmeticExpression::infix() {
    infix(root);
}

void arithmeticExpression::prefix() {
    prefix(root);
}

void arithmeticExpression::postfix() {
    postfix(root);
}

void arithmeticExpression::visualizeTree(ofstream &, TreeNode *) {
    cout << "(RECURSIVE)VISUALTREE(): STILL NEED TO WORK ON" << endl;
}

arithmeticExpression::arithmeticExpression(const string & input) {
    root = NULL;
    infixExpression = input;
}

void arithmeticExpression::buildTree() {
    infixExpression = infix_to_postfix();
    char key = 'a';

    stack<TreeNode*> stack;

    for(unsigned int i = 0; i < infixExpression.size(); ++i) {
        if(priority(infixExpression[i]) == 0) {
            TreeNode* child = new TreeNode(infixExpression[i], key);
            stack.push(child);
            ++key;
        } else {
            TreeNode* parent = new TreeNode(infixExpression[i], key);
            parent->right = stack.top();
            stack.pop();
            parent->left = stack.top();
            stack.pop();

            stack.push(parent);
            ++key;
        }
    }

    root = stack.top();
    stack.pop();
}

void arithmeticExpression::infix(TreeNode * start) {
    TreeNode* curr = start;

    if(curr->left == NULL && curr->right == NULL) {
        cout << curr->data;
    } else {
        cout << "(";
        infix(curr->left);
        cout << curr->data;
        infix(curr->right);
        cout << ")";
    }
}

void arithmeticExpression::prefix(TreeNode * start) {
    TreeNode* curr = start;

    if(curr->left == NULL && curr->right == NULL) {
        cout << curr->data;
    } else {
        cout << curr->data;

        prefix(curr->left);
        prefix(curr->right);
    }
}

void arithmeticExpression::postfix(TreeNode * start) {
    TreeNode* curr = start;

    if(curr->left == NULL && curr->right == NULL) {
        cout << curr->data;
    } else {
        postfix(curr->left);
        postfix(curr->right);

        cout << curr->data;
    }
}