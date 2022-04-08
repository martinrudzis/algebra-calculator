/*
ITokStream.h
Created by Martin Rudzis
5/31/2021
*/

#pragma once

#include <iostream>
#include <istream>
#include <iterator>

using namespace std;

enum class ttype { operatr, assign, variable, number, lparen, rparen, linend, end };

struct token {
  ttype type_;
  string value_;

  // Function purpose: Token constructor with default parameters
  // Parameters: token, ttype
  // Preconditions: N/A
  // Postconditions: Token with initilized data members
  // Return value: N/A
  // Functions called: N/A
  token(ttype t = ttype::number, string v = "0");
};

class ITokStream {

public:

  // Function purpose: ITokStream constructor 
  // Parameters: istream
  // Preconditions: N/A
  // Postconditions: Initilized ITokStream
  // Return value: N/A
  // Functions called: N/A
  ITokStream(istream &input);

  // Function purpose: To read an ITokStream into a token
  // Parameters: Token right-hand argument
  // Preconditions: Initilized ITokStream
  // Postconditions: Token constructed from stream
  // Return value: bool
  // Functions called: N/A
  bool operator>>(token& rhs);

  // Function purpose: To allow the stream to be used a bool
  // Parameters: N/A
  // Preconditions: Initilized ITokStream
  // Postconditions: Bool reflecting stream state
  // Return value: bool
  // Functions called: N/A
  explicit operator bool() const;

private:

  int tokenCount_;
  int size_;
  istream& is_;
  istream_iterator<char> itr;

};