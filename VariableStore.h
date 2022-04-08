#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "ITokStream.h"

using namespace std;

class VariableStore {

private:

  map<string, vector<token>> variables_;

public:

  bool addVariable(string s, vector<token> expr);
  vector<token>& getExpression(string s);
  bool isDefined(token t) const;
  
};