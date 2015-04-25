#pragma once
#include <list>
#include <vector>
namespace def
{
	enum class term_kind_e
	{
		atom,
		structure,
		variable,
		reference,
		rule

	};

	struct term
	{
		term_kind_e kind;
	};

	typedef std::vector<term*> terms;

	struct atom :
		public term
	{
		std::string name;
	};

	struct structure :
		public term
	{
		terms terms;
	};

	struct variable :
		public term
	{
		unsigned int index;
	};

	struct reference :
		public term
	{
		term* item;
		reference() : item(nullptr) { kind = term_kind_e::reference; };
	};

	typedef std::vector<reference> context;

	struct rule:
		public structure
	{
		std::vector<std::string> variables;
		std::list<context*> contexts;
	};

	struct step
	{
		structure* a;
		structure* b;
		int ai;
		int bi;
		int index;

		step() : a(nullptr), b(nullptr), ai(0), bi(0){};
		step(structure* a, structure* b, int ai, int bi,int index) : a(a), b(b), ai(ai), bi(bi),index(index){};

	};
}