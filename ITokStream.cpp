/*
ITokStream.cpp
Created by Martin Rudzis
5/31/2021
*/

#include "ITokStream.h"

token::token(ttype t, string v) {
  type_ = t;
  value_ = v;
}

ITokStream::ITokStream(istream &input) : is_(input) {
  itr = is_;
}

ITokStream::operator bool() const {
  return (is_ && (*itr != '#' && *itr != '.'));
}

bool ITokStream::operator>>(token &rhs) {
  string item;
  if (isdigit(*itr)) {
    while (isdigit(*itr)) {
      item.push_back(*itr);
      itr++;
    }
    rhs.type_ = ttype::number;
  }
  else if (*itr == ':') {
    item.push_back(*itr);
    itr++;
    item.push_back(*itr);
    itr++;
    rhs.type_ = ttype::assign;
  }
  else if (*itr == '#') {
    rhs.type_ = ttype::linend;
    item.push_back(*itr);
  }
  else if (*itr == '.') {
    rhs.type_ = ttype::end;
    item.push_back(*itr);
  }
  else if (*itr == '+' || *itr == '-' || *itr == '*' || *itr == '/' || *itr == '^') {
    rhs.type_ = ttype::operatr;
    item.push_back(*itr);
    itr++;
  }
  else if (*itr == '(') {
    rhs.type_ = ttype::lparen;
    item.push_back(*itr);
    itr++;
  }
  else if (*itr == ')') {
    rhs.type_ = ttype::rparen;
    item.push_back(*itr);
    itr++;
  }
  else if (isalpha(*itr)) {
    item.push_back(*itr);
    rhs.type_ = ttype::variable;
    itr++;
  }
  else {
    cout << "Error: invalid input " << *itr << endl;
  }
  rhs.value_ = item;
  return bool();
}