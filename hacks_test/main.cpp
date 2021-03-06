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
	
//	Expression e(rnd_func(types(flags), 0, height));
//	cout << "Evaluate $$\\int" << diff(e) << "dx$$\n" <<  e << "" << endl;
//	Expression de = diff(e);
	
	
	set<Node*> c;
	c.insert(new RationalNode(Rational(1, 13)));
	c.insert(new IdentityNode);
	c.insert(new IdentityNode);
	set<Node*> cc;
	cc.insert(new ProductNode(c));
	cc.insert(new ExpNode(new IdentityNode, new RationalNode(Rational(-7, 9))));
	set<Node*> c1;
	c1.insert(new RationalNode(Rational(-1, 1)));
	c1.insert(new AdditionNode(cc));
	Expression e(new LogNode(new LogNode(new ProductNode(c))));
//	Expression de(new InversionNode(new ProductNode(c)));
	Expression de = diff(e);
	
	/*Expression inv2(new InversionNode(new InversionNode(new PatternMatchNode(0))));
	Expression lolnothing(new PatternMatchNode(0));
	

	Rule D2(lolnothing, inv2);*/
	
	cout << e << "\n\n" << de << "\n";
//	cout << de << "\n";
	vector <Rule*> rules = generateSimps();
	Expression dee = Simplify(rules,de);
//	vector <Expression> v = D2.Apply(de);
	cout << "\n\n\n" << dee << "\n";
//	cout << measure(v[0].head, 0) << " " << measure(dee.head, 0) << "\n";
//	cout << "\n\n\n" << measure(e.head) << "\n";
	int x = 1;
	x++;
	return 0;
}

