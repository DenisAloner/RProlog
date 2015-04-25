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

	struct reference :
		public structure
	{
		std::string name;
		reference(std::string name) : name(name) { kind = term_kind_e::reference; terms.push_back(nullptr); };
	};

	struct variable :
		public term
	{
		reference* item;
	};

	typedef std::vector<reference*> context;

	struct rule:
		public structure
	{
		context context;
	};

	struct binding
	{
		reference* ref;
		binding(reference* ref) : ref(ref){};
	};

	struct step
	{
		structure* a;
		structure* b;
		int ai;
		int bi;
		int index;
		binding* bind;

		step() : a(nullptr), b(nullptr), ai(0), bi(0){};
		step(structure* a, structure* b, int ai, int bi, int index, binding* bind) : a(a), b(b), ai(ai), bi(bi), index(index),bind(bind){};

	};

	std::string WriteTerm(def::term* a);
	
}