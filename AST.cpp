/*
AST.cpp
Created by Martin Rudzis
5/31/2021
*/

#include "AST.h"

Node::Node() {
  token t;
  data_ = t;
  left = nullptr;
  right = nullptr;
}

Node::~Node() {
  delete left;
  delete right;
}

Node::Node(token t){
  data_ = t;
  left = nullptr;
  right = nullptr;
}

AST::AST() {
  root_ = nullptr;
}

AST::AST(vector<token>& postfixExpr) {
  stack<Node*> buildHelper;
  for (token t : postfixExpr) {
    if (isdigit(t.value_[0]) || isalpha(t.value_[0]) || t.value_.size() > 1) {
      Node* n = new Node(t);
      buildHelper.push(n);
    }
    else {
      Node* n = new Node(t);
      n->left = buildHelper.top();
      buildHelper.pop();
      n->right = buildHelper.top();
      buildHelper.pop();
      buildHelper.push(n);
    }
  }
  root_ = buildHelper.top();
}

AST::~AST() {
  delete root_;
}

Node* AST::getRoot() {
  return root_;
}

void AST::gatherData(Node* root, VariableStore &vars, stack<token>& postfix) {
  if (root != nullptr) {
    gatherData(root->left, vars, postfix);
    gatherData(root->right, vars, postfix);
    if (root->data_.type_ == ttype::variable && vars.isDefined(root->data_)) {
      AST subAST(vars.getExpression(root->data_.value_));
      gatherData(subAST.root_, vars, postfix);
    }
    if (root->data_.type_ == ttype::number || (root->data_.type_ == ttype::variable && !vars.isDefined(root->data_))) {
      if (root->data_.type_ == ttype::variable) {
        postfix.push(token(ttype::variable, root->data_.value_));
      }
      else {
        postfix.push(token(ttype::number, root->data_.value_));
      }
    }
    if (root->data_.type_ == ttype::operatr) {
      token rt = postfix.top();
      string r = rt.value_;
      postfix.pop();
      token lt = postfix.top();
      string l = lt.value_;
      postfix.pop();
      if ((isdigit(r[0]) && isdigit(l[0])) || r.size() > 1 || l.size() > 1) {
        int result;
        switch(root->data_.value_[0]) {
          case '+':
            result = stoi(r) + stoi(l);
            break;
          case '-':
            result = stoi(r) - stoi(l);
            break;
          case '*':
            result = stoi(r) * stoi(l);
            break;
          case '/':
            result = stoi(r) / stoi(l);
            break;
          case '^':
            result = pow(stoi(r), stoi(l));
        }
        token value(ttype::number, to_string(result));
        postfix.push(value);
      }
      else {
        postfix.push(lt);
        postfix.push(rt);
        token op(ttype::operatr, root->data_.value_);
        postfix.push(op);
      }   
    }
  }
}

AST* AST::simplify(VariableStore &vars) {
  vector<token>::iterator it;
  vector<token> postfixVector;
  stack<token> postfix;
  stack<token> reverse;
  gatherData(this->root_, vars, postfix);
  it = postfixVector.begin();
  while (!postfix.empty()) {
    reverse.push(postfix.top());
    postfix.pop();
  }
  while (!reverse.empty()) {
    postfixVector.push_back(reverse.top());
    reverse.pop();
  }
  for (token t : postfixVector) {
  }
  AST* newAST = new AST(postfixVector);
  return newAST;
}

void AST::getParentheses(Node* root, stack<string>& result) const {
  if (root != nullptr) {
    getParentheses(root->right, result);
    getParentheses(root->left, result);
    if ((isdigit(root->data_.value_[0]) || isalpha(root->data_.value_[0])) || root->data_.value_.size() > 1) {
      result.push(root->data_.value_);
    }
    else {
      string l = result.top();
      result.pop();
      l += " " + root->data_.value_;
      string r = result.top();
      result.pop();
      l += " " + r;
      string lparen = "(";
      lparen += l;
      lparen += ")";
      result.push(lparen);
    }
  }
}

string AST::toInfix() const {
  stack<string> result;
  getParentheses(root_, result);
  string infix = result.top();
  if (infix[0] == '(' && infix[infix.size() - 1] == ')') {
    infix = infix.substr(1, infix.size() - 2);
  }
  return infix;
}