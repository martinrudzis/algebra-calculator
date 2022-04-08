  /*
AST.h
Created by Martin Rudzis
5/31/2021
*/

#pragma once

#include <iostream>
#include "ITokStream.h"
#include "VariableStore.h"
#include <stack>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

struct Node {
  token data_;
  Node* left;
  Node* right;
  // Function purpose: Node constructor
  // Parameters: N/A
  // Preconditions: N/A
  // Postconditions: Node with pointers initiliazed to nullptr
  // Return value: N/A
  // Functions called: N/A
  Node();

  // Function purpose: Node destructor
  // Parameters: N/A
  // Preconditions: Dynamically-allocated Node
  // Postconditions: Memory is freed
  // Return value: N/A
  // Functions called: N/A
  ~Node();
  
  // Function purpose: Node constructor with token parameter to initialize to token data member
  // Parameters: token
  // Preconditions: N/A
  // Postconditions: Node with pointers initiliazed to nullptr, data_ initialized to token
  // Return value: N/A
  // Functions called: N/A
  Node(token t);
};

class AST {

public:
  // Function purpose: AST constructor
  // Parameters: N/A
  // Preconditions: N/A
  // Postconditions: An AST with root_ initilized to nullptr
  // Return value: N/A
  // Functions called: N/A
  AST();

  // Function purpose: To construct an AST from a vector of tokens in postfix order
  // Parameters: vector<token>
  // Preconditions: N/A
  // Postconditions: AST containing expression in vector
  // Return value: N/A
  // Functions called: N/A
  AST(vector<token>& postfixExpr);

  // Function purpose: AST destructor
  // Parameters: N/A
  // Preconditions: Dynamically allocated AST
  // Postconditions: Memory block is freed
  // Return value: N/A
  // Functions called: N/A 
  ~AST();

   // Function purpose: To return a pointer to the root of the AST
  // Parameters: N/A
  // Preconditions: N/A
  // Postconditions: Access to root of tree
  // Return value: Node*
  // Functions called: N/A
  Node* getRoot();

  // Function purpose: To construct a deep copy of an AST that simplifed as much as possible
  // Parameters: VariableStore
  // Preconditions: Initialized AST
  // Postconditions: AST containing the result of simplification
  // Return value: AST*
  // Functions called: gatherData()
  AST* simplify(VariableStore &vars);

  // Function purpose: To create an infix string, with appropriate parentheses, from a simplified AST
  // Parameters: Node*, stack<string>
  // Preconditions: Initilized AST
  // Postconditions: stack parameter contains string infix expression for simplified AST
  // Return value: N/A
  // Functions called: N/A - recursive
  void getParentheses(Node* root, stack<string>& result) const;

  // Function purpose: To create an infix string representation from the AST
  // Parameters: N/A
  // Preconditions: Initilized AST
  // Postconditions: infix expression for the AST contents
  // Return value: string
  // Functions called: getParentheses()
  string toInfix() const;
  
private:

  // Function purpose: To create a postfix stack containing the result of simplifying the AST
  // Parameters: Node*, VariableStore, stack<token>
  // Preconditions: Initilized AST
  // Postconditions: stack parameter containing postfix expression for simplified AST
  // Return value: N/A
  // Functions called: N/A - recursive
  void gatherData(Node* root, VariableStore &vars, stack<token>& postfix);
  Node* root_;

};