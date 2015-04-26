#include <iostream>
#include "Parser.h"
#include "Solver.h"

int main() {
	std::cout.setf(std::ios::boolalpha);
	Parser p;
	Solver s;
	def::term* program = p.parse("[program [> 4 1 4] [> 3 2 8] [> 1 4 2] [> 4 3 5] [> 4 4 2]]");
	def::term* query = p.parse("[[query] [> 4 ?x 5] [> ?x 4 2]]");
	/*def::atom* a=new def::atom;
	a->kind = def::term_kind_e::atom;
	a->name = "yes";
	static_cast<def::rule*>(query)->context[0]->terms[0] = a;
	std::cout << static_cast<def::rule*>(query)->context.size() << std::endl;*/
	std::cout << def::WriteTerm(program) << std::endl;
	std::cout << def::WriteTerm(query) << std::endl;
	std::cout << s.solve(program,query) << std::endl;
	std::cout << def::WriteTerm(program) << std::endl;
	//def::reference ref;
	//def::atom a;
	//a.kind = def::term_kind_e::atom;
	//a.name = "yes";
	//ref.item = &a;
	//static_cast<def::rule*>(query)->contexts.front()[0] = ref;
	std::cout << def::WriteTerm(query) << std::endl;
	system("pause");
	return 0;
}

