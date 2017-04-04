#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include "rules.hpp"
#include "expression.hpp"
#include "rand_func.h"
#include "simplify.h"

using namespace std;

vector<NodeType> allTypes = {
	NodeType::ConstantQ,      //0
	NodeType::ConstantPi,     //0
	NodeType::ConstantE,      //0
	NodeType::Identity,       //0
	NodeType::Addition,       //n
	NodeType::Multiplication, //n
	NodeType::Negation,       //1
	NodeType::Inversion,      //1
	NodeType::Exponentiation, //2
	NodeType::Logarithm,      //1
	NodeType::Sine,           //1
	NodeType::Cosine,         //1
	NodeType::ArcSin,         //1
	NodeType::ArcTan,         //1
};

vector<NodeType> types(string f) {
	vector<NodeType> result;
	for (int i = 0; i < f.size() && i < 14; ++i) {
		if (f[i] == '1') {
			result.push_back(allTypes[i]);
		}
	}
	return result;
}



int main(int argc, char** argv) {
	srand(time(NULL));
	int height = 3;
	string flags = "11111111110000";
	
	
//	cout << flags << endl;
	
	Expression e(rnd_func(types(flags), 0, height));
//	cout << "Evaluate $$\\int" << diff(e) << "dx$$\n" <<  e << "" << endl;
	Expression de = diff(e);
	
	
/*	set<Node*> c;
//	c.insert(new RationalNode(Rational(1, 1)));
	c.insert(new RationalNode(Rational(1, 1)));
//	c.insert(new RationalNode(Rational(1, 1)));
	c.insert(new IdentityNode);
	set<Node*> c1;
	c1.insert(new InversionNode(new InversionNode(new IdentityNode)));
	c1.insert(new ProductNode(c));
	Expression de(new ProductNode(c1));*/
	
	cout << e << "\n\n" << de << "\n";
//	cout << de << "\n";
	vector <Rule*> rules = generateSimps();
	Expression dee = Simplify(rules,de);
	cout << "\n\n\n" << dee << "\n";
//	cout << "\n\n\n" << measure(e.head) << "\n";
	return 0;
}

//
//  main.cpp
//  hacks_test
//
//  Created by kostya on 4/1/17.
//  Copyright © 2017 kostya. All rights reserved.
//

/*#include <iostream>
#include <set>
#include "rules.hpp"
#include "simplify.h"

using namespace std;



int main()
{
	//	RationalNode l(Rational(1, 2)), r(Rational(1, 1));
	set<Node*> c;
	c.insert(new IdentityNode);
	c.insert(new IdentityNode);
	Expression expr(new ProductNode(c));
	
	set<Node*> cc;
	Expression empt(new AdditionNode(cc));
	
	set<Node*> cp;
	cp.insert(new PatternMatchNode(0));
	cp.insert(new NegationNode(new PatternMatchNode(0)));
	Expression amina(new AdditionNode(cp));
	
	set<Node*> ra2;
	for(int i = 0; i < 2; i++){
		ra2.insert(new PatternMatchNode(0));
	}
	Expression reppr(new ProductNode(ra2));
	Expression natexp(new ExpNode(new PatternMatchNode(0), new RationalNode(Rational(2,1)) ));
	Rule R13(reppr,natexp);
	Rule R14(natexp,reppr);
	
//	vector<pair<int, Expression>> replace_tester;
//	replace_tester.push_back(make_pair(0, Expression(new RationalNode(Rational(1,1)))));
	
//	cout << replace(replace_tester, amina) << endl;
	
	vector <Expression> v = R13.Apply(expr);
//	vector <vector <pair <int, Expression>>> v1 = PatternList(expr, amina);
	cout << v.size() << "\n";
	//vector <vector <pair <int, Expression>>> v = PatternList(expr, exprp);
//	cout << (amina.head)->isEqual(amina1.head) << "\n";
//	cout << (expr.head)->isEqual(expr1.head) << "\n";
//	v1 = PatternList(expr1, amina1);
//	cout << v1.size() << "\n";
	//	for(int i = 0; i < v.size(); i++)
	//		cout << v[i].size() << " ";
	//	cout << "\n";
	return 0;
}*/


