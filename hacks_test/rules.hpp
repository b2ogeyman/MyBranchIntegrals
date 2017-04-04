#ifndef rules_h
#define rules_h

#include "expression.hpp"
#include <vector>

using namespace std;

std::vector<std::vector<std::pair<int, Expression>>> PatternList(const Expression& formula, const Expression& pattern);

Expression replace(vector<pair<int, Expression>> replacements, Expression Base);

class Rule{
public:
	Rule(const Expression& s, const Expression& f) : start(s), finish(f) {}
	Rule(Node* s, Node* f) : start(Expression(s)), finish(Expression(f)) {}
	std::vector<Expression> Apply(const Expression& formula);
private:
	Expression start;
	Expression finish;
};

#endif
