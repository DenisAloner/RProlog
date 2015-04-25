#include <iostream>
#include "Parser.h"
#include "Solver.h"

std::string WriteTerm(def::term* a, def::rule* r);

int main() {
	std::cout.setf(std::ios::boolalpha);
	Parser p;
	Solver s;
	def::term* program = p.parse("[program [> 4 1 4] [> 4 3 5]]");
	def::term* query = p.parse("[[query] [> 4 ?x 5]]");
	std::cout << WriteTerm(program, nullptr) << std::endl;
	std::cout << WriteTerm(query, nullptr) << std::endl;
	std::cout << s.solve(program,query) << std::endl;
	std::cout << WriteTerm(program, nullptr) << std::endl;
	def::reference ref;
	def::atom a;
	a.kind = def::term_kind_e::atom;
	a.name = "yes";
	ref.item = &a;
	//static_cast<def::rule*>(query)->contexts.front()[0] = ref;
	std::cout << WriteTerm(query, nullptr) << std::endl;
	system("pause");
	return 0;
}

std::string WriteTerm(def::term* a,def::rule* r)
{
	switch (a->kind)
	{
	case def::term_kind_e::atom:
		return static_cast<def::atom*>(a)->name;
	case def::term_kind_e::structure:
	{
		std::string result = "";
		result += "[";
		if (!static_cast<def::structure*>(a)->terms.empty())
		{
			for (auto CurrentTerm : static_cast<def::structure*>(a)->terms)
			{
				result += WriteTerm(CurrentTerm,r);
				if (CurrentTerm != static_cast<def::structure*>(a)->terms.back())
				{
					result += " ";
				}
			}
		}
		result += "]";
		return result;
	}
	case def::term_kind_e::rule:
	{
		std::string result = "";
		result += "[";
		if (!static_cast<def::rule*>(a)->terms.empty())
		{
			for (auto CurrentTerm : static_cast<def::rule*>(a)->terms)
			{
				result += WriteTerm(CurrentTerm, static_cast<def::rule*>(a));
				if (CurrentTerm != static_cast<def::rule*>(a)->terms.back())
				{
					result += " ";
				}
			}
		}
		result += "]";
		return result;
	}
	case def::term_kind_e::variable:
		if (r->contexts.empty())
		{
			return "?" + r->variables[static_cast<def::variable*>(a)->index];
		}
		else
		{
			if ((*r->contexts.front())[static_cast<def::variable*>(a)->index].item)
			{
				return WriteTerm((*r->contexts.front())[static_cast<def::variable*>(a)->index].item, r);
			} else
			{
				return "?" + r->variables[static_cast<def::variable*>(a)->index];
			}
		}
	}
};