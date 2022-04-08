#include "VariableStore.h"

bool VariableStore::addVariable(string s, vector<token> expr) {
  if (variables_.size() < 26) {
    if (variables_.count(s) == 0) {
      variables_.emplace(s, expr);
    }
    else {
      variables_[s] = expr;
    }
    return true;
  }
  return false;
}

vector<token>& VariableStore::getExpression(string s) {
  return variables_[s];
}

bool VariableStore::isDefined(token t) const {
  return variables_.count(t.value_) == 1;
}
