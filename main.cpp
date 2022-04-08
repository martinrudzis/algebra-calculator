/*
main.cpp
Created by Martin Rudzis
5/31/2021
*/

/*
Program description
Specification
The goal of this project is to create a symbolic algebra calculator that can evaluate and simplify algebraic expressions that are read from cin.

Input
It is assumed that input is provided in infix format. The program will read infix expressions from cin. Expressions may use any of the 26 letters of the alphabet for variables, and any of the following operators: +, -, *, /, ^. Assignment is performed using the operator :=. The calculator will not differentiate between upper and lower case; upper and lower case forms of the same letter will be treated as belonging to the same variable. The program will also detect parentheses in the input and rely on them to determine order of operations. The program ends when it reads a “.” from cin.

Output
The output of the program will be as follows. It will print the input that it reads in, followed by the simplified form of the output. Each new input and output will be printed on its own line and assigned an index number so that they can be easily compared. For example, if the input was “1 + 2”, it would print, “in [1]: 1 + 2.” Then it would print, “out [1]: 3”.   

Assumptions
It is assumed that digits that are not separated by an operator can be concatenated into a single number. For example, 5 2 + 3 would become 52 + 3. It is also assumed that numbers and variables not separated by an operator is an invalid input. For example, “2x”, “xy”, or “3(z)” would be considered invalid. Finally, it is assumed that dangling operators without left and/or right arguments are invalid. It is also assumed that assignment inputs having more than a single left argument are invalid, for example, 2 * x := 4. Use of any of these expressions should result in an error message, but still allow program execution to continue. 

Key Algorithms
There are several important algorithms involved with implementing this program. First, the program will need to convert infix mathematical expressions to postfix format. This process involves removing parentheses and converting the order of operators and numbers/variables such that they can be read into a tree structure and simplified. The tree structure will need an algorithm that simplifies the expression that it contains by performing operations on constants and simplifying defined variables. Finally, it will need an algorithm to convert the contents of the tree into a readable infix expression that can be printed to show the result of the simplification. The details of these algorithms are described in the Design section.



Design

Functions
The program driver will start by reading from cin. If the input is just a number or a variable, then it just outputs the number. However, the variable is added to the VariableStore with an empty value. If the input contains an assignment operator (:=), it will “tokenize” the right hand elements by using them to create an ITokStream, which is then read element-by-element into a series of Nodes that are stored in a vector. The program will then utilize a function to convert the infix expression to a postfix format. The implementation of this function will be as follows (note that a helper function, getPrecedence(Token), will be defined such that it returns an int corresponding to the precedence of each operator):

*/

#include <iostream>
#include <vector>
#include <stack>
#include <assert.h>
#include "ITokStream.h"
#include "AST.h"
#include "VariableStore.h"
#include <sstream>

using namespace std;


int getPrecedence(string s) {
  char c = s[0];
  switch(c) {
    case '+': case '-':
      return 1;
    case '*': case '/':
      return 2;
    case '^':
      return 3;
  }
  return 0;
}

vector<token> infixToPostfix(vector<token> &input) {
  vector<token> postfix;
  stack<token> s;
  for (token t : input) {
    if (t.type_ == ttype::lparen) {
      s.push(t);
    }
    else if (t.type_ == ttype::operatr) {
      while (!s.empty() && s.top().type_ != ttype::lparen && getPrecedence(t.value_) <= getPrecedence(s.top().value_)) {
        postfix.push_back(s.top());
        s.pop();
      }
      s.push(t);
    }
    else if (t.type_ == ttype::rparen) {
      while(s.top().type_ != ttype::lparen) {
        postfix.push_back(s.top());
        s.pop();
      }
      s.pop();
    }
    else {
      postfix.push_back(t);
    }
  }
  while(!s.empty()) {
    postfix.push_back(s.top());
    s.pop();
  }
  return postfix;
}


bool checkForAssignment(vector<token> infix, VariableStore& vars) {
  int index = 0;
  vector<token> value;
  for (token t : infix) {
    if (t.type_ == ttype::assign) {
      string key = infix[index - 1].value_;
      for (int i = index + 1; i < infix.size() - 1; i++) {
        cout << infix[i].value_ << " ";
        value.push_back(infix[i]);
      }
      cout << endl;
      value = infixToPostfix(value);
      vars.addVariable(key, value);
      vector<token> result = vars.getExpression(key);
      return true;
    }
    index++;
  }
  return false;
}


int main() {

  VariableStore vars;
  vector<token> infix;

  token t;
  string input;
  int count = 0;
  while(t.type_ != ttype::end) {
    getline(cin, input);
    cout << "in  [" << count << "]: " << input << endl;
    cout << "out [" << count << "]: ";
    count++;
    input.push_back('#');
    istringstream iss(input);
    ITokStream its(iss);
    while (t.type_ != ttype::end && t.type_ != ttype::linend) {
      its >> t;
      infix.push_back(t);
    }
    bool hasAssignment = checkForAssignment(infix, vars);
    if (hasAssignment) {
      infix.clear();
      t = token();
    }
    else if (t.type_ != ttype::end) {
      infix.pop_back();
      infix = infixToPostfix(infix);
      AST myTree(infix);

      AST* newTree = myTree.simplify(vars);
      cout << newTree->toInfix();
      delete newTree;
      newTree = nullptr;

      cout << endl;
      infix.clear();
      t = token();
    }
  }

}