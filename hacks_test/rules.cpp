#include "rules.hpp"
#include <iostream>

using namespace std;

vector<vector<pair<int, Expression>>> PatternList(const Expression& formula, const Expression& pattern) {
	vector<vector<pair<int, Expression>>> list;
	
	if (formula.head->Type() != pattern.head->Type()
		|| pattern.head->arity() != formula.head->arity()){
		if(pattern.head->Type() == NodeType::PatternMatch){
			vector<pair <int, Expression>> singleton;
			singleton.push_back(make_pair(dynamic_cast<PatternMatchNode*>(pattern.head)->getIndex(),formula));
			list.push_back(singleton);
		}else{
			return list;
		}
	}
	
	else if (formula.head->arity() == 0){
		if(*(formula.head) == *(pattern.head)){
			vector<pair<int, Expression>> xxx;
			list.push_back(xxx);
		}
	}
	
	else if (formula.head->Type() == NodeType::Addition) {
		set <Node*> FormulaList = dynamic_cast<AdditionNode*>(formula.head)->addends;
		set <Node*> Pattern_List = dynamic_cast<AdditionNode*>(pattern.head)->addends;
		Node* PatternFirst = *((dynamic_cast<AdditionNode*>(pattern.head)->addends).begin());
		for(set<Node*>::iterator it = FormulaList.begin(); it != FormulaList.end(); ++it){
			Node* elmt = *it;
			if((elmt->arity() == PatternFirst->arity()
				&& elmt->Type() == PatternFirst->Type())
			   || PatternFirst->Type()== NodeType::PatternMatch){
				
				
				Expression formulaClone = formula;
				Expression patternClone = pattern;
				
				for(set<Node*>::iterator jt = (dynamic_cast<AdditionNode*>(formulaClone.head)->addends).begin(); jt != (dynamic_cast<AdditionNode*>(formulaClone.head)->addends).end(); ++jt)
					if((*it)->isEqual(*jt))
					{
						(dynamic_cast<AdditionNode*>(formulaClone.head)->addends).erase(jt);
						break;
					}
				
				for(set<Node*>::iterator jt = (dynamic_cast<AdditionNode*>(patternClone.head)->addends).begin(); jt != (dynamic_cast<AdditionNode*>(patternClone.head)->addends).end(); ++jt)
					if(PatternFirst->isEqual(*jt))
					{
						(dynamic_cast<AdditionNode*>(patternClone.head)->addends).erase(jt);
						break;
					}
				
				
				
				Expression expr1(elmt->clone());
				Expression expr2(PatternFirst->clone());

				
				vector<vector<pair <int, Expression>>> V = PatternList(expr1, expr2);
				vector<vector<pair <int, Expression>>> U = PatternList(formulaClone, patternClone);
				
				for(int i = 0; i < V.size(); i++){
					for(int j = 0; j < U.size(); j++){
						vector <int> was(100, 0);
						vector <pair<int, Expression>> v = V[i], u = U[j];
						vector<pair <int, Expression>> add;
						bool flag = true;
						for(int ii = 0; ii < v.size(); ii++)
							for(int jj = 0; jj < u.size(); jj++)
							{
								pair<int, Expression> x = v[ii], y = u[jj];
								if(x.first == y.first)
									if(x.second != y.second)
										flag = false;
							}
									
						for(int ii = 0; ii < v.size(); ii++)
						{
							pair<int, Expression> x = v[ii];
							if(!was[x.first])
							{
								was[x.first] = 1;
								add.push_back(x);
							}
						}
						for(int jj = 0; jj < u.size(); jj++)
						{
							pair<int, Expression> y = u[jj];
							if(!was[y.first])
							{
								was[y.first] = 1;
								add.push_back(y);
							}
						}
						

						if(flag)
							list.push_back(add);
					}
				}
			}
		}
	}
	
	else if (formula.head->Type() == NodeType::Multiplication) {
		auto FormulaList = dynamic_cast<ProductNode*>(formula.head)->factors;
		auto PatternFirst = *((dynamic_cast<ProductNode*>(pattern.head)->factors).begin());
		for(set<Node*>::iterator it = FormulaList.begin(); it != FormulaList.end(); ++it){
			Node* elmt = *it;
			if((elmt->arity() == PatternFirst->arity()
			  && elmt->Type() == PatternFirst->Type())
			 || PatternFirst->Type()== NodeType::PatternMatch){
				
				Expression formulaClone = formula;
				Expression patternClone = pattern;
				
				for(set<Node*>::iterator jt = (dynamic_cast<ProductNode*>(formulaClone.head)->factors).begin(); jt != (dynamic_cast<ProductNode*>(formulaClone.head)->factors).end(); ++jt)
					if((*it)->isEqual(*jt))
					{
						(dynamic_cast<ProductNode*>(formulaClone.head)->factors).erase(jt);
						break;
					}
				
				for(set<Node*>::iterator jt = (dynamic_cast<ProductNode*>(patternClone.head)->factors).begin(); jt != (dynamic_cast<ProductNode*>(patternClone.head)->factors).end(); ++jt)
					if(PatternFirst->isEqual(*jt))
					{
						(dynamic_cast<ProductNode*>(patternClone.head)->factors).erase(jt);
						break;
					}
				
				vector<vector<pair <int, Expression>>> V = PatternList(Expression(elmt->clone()), Expression(PatternFirst->clone()));
				vector<vector<pair <int, Expression>>> U = PatternList(formulaClone,patternClone);
				
				for(const auto& v: V){
					for(const auto& u: U){
						vector <int> was(100, 0);
						vector<pair <int, Expression>> add;
						bool flag = true;
						for(const auto& x : v)
							for(const auto& y : u)
								if(x.first == y.first)
									if(x.second != y.second)
										flag = false;
						
						for(const auto& x : v)
							if(!was[x.first])
							{
								was[x.first] = 1;
								add.push_back(x);
							}
						for(const auto& y : u)
							if(!was[y.first])
							{
								was[y.first] = 1;
								add.push_back(y);
							}
						
						if(flag)
							list.push_back(add);
						
					}
				}
			}
		}
	}
	
	else if (formula.head->Type() == NodeType::Exponentiation){
		
		vector<vector<pair <int, Expression>>> V = PatternList(Expression(dynamic_cast<ExpNode*>(formula.head)->base->clone()), Expression(dynamic_cast<ExpNode*>(pattern.head)->base->clone()));
		vector<vector<pair <int, Expression>>> U = PatternList(Expression(dynamic_cast<ExpNode*>(formula.head)->exponent->clone()), Expression(dynamic_cast<ExpNode*>(pattern.head)->exponent->clone()));
		
		for(const auto& v: V){
			for(const auto& u: U){
				vector <int> was(100, 0);
				vector<pair <int, Expression>> add;
				bool flag = true;
				for(const auto& x : v)
					for(const auto& y : u)
						if(x.first == y.first)
							if(x.second != y.second)
								flag = false;
				
				for(const auto& x : v)
					if(!was[x.first])
					{
						was[x.first] = 1;
						add.push_back(x);
					}
				for(const auto& y : u)
					if(!was[y.first])
					{
						was[y.first] = 1;
						add.push_back(y);
					}
				
				if(flag)
					list.push_back(add);
				
			}
		}

		
	}
	
	else if (formula.head->isStrictArity1()){
		list = PatternList(Expression(dynamic_cast<Arity1Node*>(formula.head)->getArg()->clone()), Expression(dynamic_cast<Arity1Node*>(pattern.head)->getArg()->clone()));
	}
	
	return list;
}


///////////////////////////////////////////////////////////////////////////////////



Expression replace(vector<pair<int, Expression>> replacements, Expression Base){
	if(Base.head->Type() == NodeType::PatternMatch){
		for(const auto& v:replacements){
			if(v.first == dynamic_cast<PatternMatchNode*>(Base.head)->getIndex()){
				return v.second;
			}
		}
	}
	else if(Base.head->Type() == NodeType::Addition){
		Expression baseClone = Base;
		set <Node*> s;
		for(set<Node*>::iterator it = (dynamic_cast<AdditionNode*>(baseClone.head)->addends).begin(); it != (dynamic_cast<AdditionNode*>(baseClone.head)->addends).end(); ++it){
			Expression expr((*it)->clone());
			Expression y = replace(replacements, expr);
			s.insert(y.head->clone());
		}
		(dynamic_cast<AdditionNode*>(baseClone.head)->addends).clear();
		for(set<Node*>::iterator it = s.begin(); it != s.end(); ++it){
			(dynamic_cast<AdditionNode*>(baseClone.head)->addends).insert((*it)->clone());
		}
		return baseClone;
	}
	else if(Base.head->Type() == NodeType::Multiplication){
		Expression baseClone = Base;
		set <Node*> s;
		for(set<Node*>::iterator it = (dynamic_cast<ProductNode*>(baseClone.head)->factors).begin(); it != (dynamic_cast<ProductNode*>(baseClone.head)->factors).end(); ++it){
			s.insert(replace(replacements, Expression((*it)->clone())).head->clone());
		}
		(dynamic_cast<ProductNode*>(baseClone.head)->factors).clear();
		for(set<Node*>::iterator it = s.begin(); it != s.end(); ++it){
			(dynamic_cast<ProductNode*>(baseClone.head)->factors).insert((*it)->clone());
		}
		return baseClone;
	}
	else if(Base.head->Type() == NodeType::Exponentiation){
		Expression baseClone = Base;
		Node* child1 = dynamic_cast<ExpNode*>(baseClone.head)->base;
		Node* child2 = dynamic_cast<ExpNode*>(baseClone.head)->exponent;
		return Expression(new ExpNode(replace(replacements, Expression(child1->clone())).head->clone(), replace(replacements, Expression(child2->clone())).head->clone()));
	}
	else if(Base.head->isStrictArity1()){
		Expression baseClone = Base;
		Node* child = dynamic_cast<Arity1Node*>(Base.head)->getArg()->clone();
		dynamic_cast<Arity1Node*>(baseClone.head)->setArg(replace(replacements, Expression(child)).head->clone());
//		Expression baseClone = replace(replacements, Expression(child));
		return baseClone;
	}
	return Base;
}

Expression contract_rationals(Expression Base)
{
	if(Base.head->Type() == NodeType::Addition)
	{
		Expression baseClone = Base;
		set <Node*> s;
		Rational sum(0, 1);
		for(set<Node*>::iterator it = (dynamic_cast<AdditionNode*>(baseClone.head)->addends).begin(); it != (dynamic_cast<AdditionNode*>(baseClone.head)->addends).end(); ++it)
		{
			if((*it)->Type() == NodeType::ConstantQ)
				sum += dynamic_cast<RationalNode*>(*it)->getNumber();
			else if((*it)->Type() == NodeType::Negation && dynamic_cast<NegationNode*>(*it)->getArg()->Type() == NodeType::ConstantQ)
				sum -= dynamic_cast<RationalNode*>(dynamic_cast<NegationNode*>(*it)->getArg())->getNumber();
			else
			{
				Expression expr((*it)->clone());
				Expression y = contract_rationals(expr);
				s.insert(y.head->clone());
			}
		}
		if(sum != Rational(0, 1))
		{
			if(sum < Rational(0, 1))
				s.insert(new NegationNode(new RationalNode(-sum)));
			else
				s.insert(new RationalNode(sum));
		}
		(dynamic_cast<AdditionNode*>(baseClone.head)->addends).clear();
		for(set<Node*>::iterator it = s.begin(); it != s.end(); ++it)
		{
			(dynamic_cast<AdditionNode*>(baseClone.head)->addends).insert((*it)->clone());
		}
		return baseClone;
	}
	else if(Base.head->Type() == NodeType::Multiplication){
		Expression baseClone = Base;
		set <Node*> s;
		Rational prod(1, 1);
		for(set<Node*>::iterator it = (dynamic_cast<ProductNode*>(baseClone.head)->factors).begin(); it != (dynamic_cast<ProductNode*>(baseClone.head)->factors).end(); ++it){
			if((*it)->Type() == NodeType::ConstantQ)
				prod *= dynamic_cast<RationalNode*>(*it)->getNumber();
			else if((*it)->Type() == NodeType::Negation && dynamic_cast<NegationNode*>(*it)->getArg()->Type() == NodeType::ConstantQ)
				prod *= -(dynamic_cast<RationalNode*>(dynamic_cast<NegationNode*>(*it)->getArg())->getNumber());
			else
			{
				Expression expr((*it)->clone());
				Expression y = contract_rationals(expr);
				s.insert(y.head->clone());
			}
		}
		if(prod != Rational(1, 1))
		{
			if(prod < Rational(0, 1))
				s.insert(new NegationNode(new RationalNode(-prod)));
			else
				s.insert(new RationalNode(prod));
		}
		(dynamic_cast<ProductNode*>(baseClone.head)->factors).clear();
		for(set<Node*>::iterator it = s.begin(); it != s.end(); ++it){
			(dynamic_cast<ProductNode*>(baseClone.head)->factors).insert((*it)->clone());
		}
		return baseClone;
	}
	else if(Base.head->Type() == NodeType::Exponentiation){
		Expression baseClone = Base;
		Node* child1 = dynamic_cast<ExpNode*>(baseClone.head)->base;
		Node* child2 = dynamic_cast<ExpNode*>(baseClone.head)->exponent;
		return Expression(new ExpNode(contract_rationals(Expression(child1->clone())).head->clone(), contract_rationals(Expression(child2->clone())).head->clone()));
	}
	else if(Base.head->isStrictArity1()){
		Expression baseClone = Base;
		Node* child = dynamic_cast<Arity1Node*>(Base.head)->getArg()->clone();
		dynamic_cast<Arity1Node*>(baseClone.head)->setArg(contract_rationals(Expression(child)).head->clone());
		//		Expression baseClone = replace(replacements, Expression(child));
		return baseClone;
	}
	return Base;
}


Expression flatten(Expression Base)
{
	if(Base.head->Type() == NodeType::Addition)
	{
		Expression baseClone = Base;
		set <Node*> s;
		for(set<Node*>::iterator it = (dynamic_cast<AdditionNode*>(baseClone.head)->addends).begin(); it != (dynamic_cast<AdditionNode*>(baseClone.head)->addends).end(); ++it)
		{
			s.insert(flatten(Expression((*it)->clone())).head->clone());
		}
		(dynamic_cast<AdditionNode*>(baseClone.head)->addends).clear();
		set <Node*> s1;
		for(set<Node*>::iterator it = s.begin(); it != s.end(); ++it)
		{
			if((*it)->Type() != NodeType::Addition)
				s1.insert((*it)->clone());
			else
				for(set<Node*>::iterator jt = (dynamic_cast<AdditionNode*>(*it)->addends).begin(); jt != (dynamic_cast<AdditionNode*>(*it)->addends).end(); ++jt)
					s1.insert((*jt)->clone());
		}
		for(set<Node*>::iterator it = s1.begin(); it != s1.end(); ++it){
			(dynamic_cast<AdditionNode*>(baseClone.head)->addends).insert((*it)->clone());
		}
		return baseClone;
	}
	else if(Base.head->Type() == NodeType::Multiplication){
		Expression baseClone = Base;
		set <Node*> s;
		for(set<Node*>::iterator it = (dynamic_cast<ProductNode*>(baseClone.head)->factors).begin(); it != (dynamic_cast<ProductNode*>(baseClone.head)->factors).end(); ++it)
		{
			s.insert(flatten(Expression((*it)->clone())).head->clone());
		}
		(dynamic_cast<ProductNode*>(baseClone.head)->factors).clear();
		set <Node*> s1;
		for(set<Node*>::iterator it = s.begin(); it != s.end(); ++it)
		{
			if((*it)->Type() != NodeType::Multiplication)
				s1.insert((*it)->clone());
			else
				for(set<Node*>::iterator jt = (dynamic_cast<ProductNode*>(*it)->factors).begin(); jt != (dynamic_cast<ProductNode*>(*it)->factors).end(); ++jt)
					s1.insert((*jt)->clone());
		}
		for(set<Node*>::iterator it = s1.begin(); it != s1.end(); ++it){
			(dynamic_cast<ProductNode*>(baseClone.head)->factors).insert((*it)->clone());
		}
		return baseClone;
	}
	else if(Base.head->Type() == NodeType::Exponentiation){
		Expression baseClone = Base;
		Node* child1 = dynamic_cast<ExpNode*>(baseClone.head)->base;
		Node* child2 = dynamic_cast<ExpNode*>(baseClone.head)->exponent;
		return Expression(new ExpNode(flatten(Expression(child1->clone())).head->clone(), flatten(Expression(child2->clone())).head->clone()));
	}
	else if(Base.head->Type() == NodeType::Inversion){
		Expression baseClone = Base;
		Node* child = dynamic_cast<Arity1Node*>(Base.head)->getArg()->clone();
		if(child->Type() != NodeType::ConstantQ)
			dynamic_cast<Arity1Node*>(baseClone.head)->setArg(flatten(Expression(child)).head->clone());
		else
			dynamic_cast<Arity1Node*>(baseClone.head)->setArg(new RationalNode(Rational(1, 1)/dynamic_cast<RationalNode*>(child)->getNumber()));
		//		Expression baseClone = replace(replacements, Expression(child));
		return baseClone;
	}
	else if(Base.head->isStrictArity1()){
		Expression baseClone = Base;
		Node* child = dynamic_cast<Arity1Node*>(Base.head)->getArg()->clone();
		dynamic_cast<Arity1Node*>(baseClone.head)->setArg(flatten(Expression(child)).head->clone());
		//		Expression baseClone = replace(replacements, Expression(child));
		return baseClone;
	}
	return Base;
}


/////////////////////////////////////////////////////////////////////////////////////


vector<Expression> Rule::Apply(const Expression& formula) {
	vector<Expression> list;
	vector <vector <pair <int, Expression>>> v = PatternList(formula, start);
	for(const auto& replacements: PatternList(formula, start)){
		Expression finish1 = finish;
		list.push_back(replace(replacements, finish1));
	}
	
	if (formula.head->Type() == NodeType::Addition){
		auto nodeAddends = dynamic_cast<AdditionNode*>(formula.head)->addends;
		for (set<Node*>::iterator it = nodeAddends.begin(); it != nodeAddends.end(); ++it) {
			for (const auto& expr : Apply(Expression((*it)->clone()))){
				Expression formulaClone = formula;
				for (set<Node*>::iterator jt = (dynamic_cast<AdditionNode*>(formulaClone.head)->addends).begin(); jt != (dynamic_cast<AdditionNode*>(formulaClone.head)->addends).end();)
					if((*it)->isEqual(*jt))
					{
						jt = (dynamic_cast<AdditionNode*>(formulaClone.head)->addends).erase(jt);
						break;
					}
					else
						++jt;
				(dynamic_cast<AdditionNode*>(formulaClone.head)->addends).insert(expr.head->clone());
				list.push_back(formulaClone);
			}
		}
	}
	
	if (formula.head->Type() == NodeType::Multiplication){
		auto nodeFactors = dynamic_cast<ProductNode*>(formula.head)->factors;
		for (set<Node*>::iterator it = nodeFactors.begin(); it != nodeFactors.end(); ++it) {
			for (const auto& expr : Apply(Expression((*it)->clone()))){
				Expression formulaClone = formula;
				for (set<Node*>::iterator jt = (dynamic_cast<ProductNode*>(formulaClone.head)->factors).begin(); jt != (dynamic_cast<ProductNode*>(formulaClone.head)->factors).end();)
					if((*it)->isEqual(*jt))
					{
						jt = (dynamic_cast<ProductNode*>(formulaClone.head)->factors).erase(jt);
						break;
					}
					else
						++jt;
				(dynamic_cast<ProductNode*>(formulaClone.head)->factors).insert(expr.head->clone());
				list.push_back(formulaClone);
			}
		}
	}
	
	if (formula.head->Type() == NodeType::Exponentiation){
		Node* Base = dynamic_cast<ExpNode*>(formula.head)->base;
		Node* Exponent = dynamic_cast<ExpNode*>(formula.head)->exponent;
		for(const auto & expr : Apply(Expression(Base->clone()))){
			list.push_back(Expression(new ExpNode(expr.head->clone(), Exponent->clone())));
		}
		for(const auto & expr : Apply(Expression(Exponent->clone()))){
			list.push_back(Expression(new ExpNode(Base->clone(), expr.head->clone())));
		}
		return list;
	}
	
	if (formula.head->isStrictArity1()) {
		Node* argument = dynamic_cast<Arity1Node*>(formula.head)->getArg()->clone();
		for(const auto & expr : Apply(Expression(argument->clone()))) {
			list.push_back(Expression(dynamic_cast<Arity1Node*>(formula.head->clone())->setArg(expr.head->clone())->clone()));
		}
		return list;
	}
	
	return list;
}
