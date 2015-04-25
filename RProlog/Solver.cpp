#include "Solver.h"

Solver::Solver()
{

}
Solver::~Solver()
{

}

bool Solver::solve(def::term* a, def::term* b)
{
	def::structure* an = static_cast<def::structure*>(a);
	def::rule* bn = static_cast<def::rule*>(b);
	def::context* c = new def::context();
	for (int i = 0; i < bn->variables.size(); i++)
	{
		c->push_back(def::reference());
	}
	bn->contexts.push_front(c);
	bc.push_front(c);
	steps.push_front(def::step(an, bn, 1, 1,0));
	return trace();
}

bool Solver::trace()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	def::term* a;
	def::term* b;
	goal = false;
	callback = false;
	while (true)
	{
		if (steps.empty())
		{
			return goal;
		}
		def::step& s = steps.front();
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Stack size:" <<steps.size() << " Callback:"<< callback <<  " Goal:"<< goal << std::endl;
		SetConsoleTextAttribute(hConsole, 128);
		if (callback)
		{
			a = s.a;
			b = s.b;
		}
		else {
			a = s.a->terms[s.ai];
			b = s.b->terms[s.bi];
		}
		switch (b->kind)
		{
		case def::term_kind_e::rule:
		{
			def::rule* bn = static_cast<def::rule*>(b);
			switch (a->kind)
			{
			case def::term_kind_e::structure:
			{
				std::cout << "structure rule " << s.ai << " " << s.bi << std::endl;
				def::structure* an = static_cast<def::structure*>(a);
				if (callback)
				{
					if (goal)
					{
						bool add_variant = false;
						if ((s.ai+1) < an->terms.size())
						{
							steps.push_front(def::step(an, bn, s.ai, s.bi,s.index));
							std::swap(steps.begin(), steps.begin()++);
							add_variant = true;
						}
						s.ai = 1;
						s.bi += 1;
						if (s.bi == bn->terms.size())
						{
							if (add_variant)
							{
								def::step& p = (*(steps.begin()++)++);
								if (p.index == 0)
								{
									return true;
								}
								steps.push_front(p);
							}
						} else {
							callback = false;
						}
					} else {
						s.ai += 1;
						if (s.ai == an->terms.size())
						{
							steps.pop_front();
						}
						else {
							callback = false;
						}
					}
				}
				else {
					if (s.bi == 0)
					{
						def::context* c = new def::context();
						for (int i = 0; i < bn->variables.size(); i++)
						{
							c->push_back(def::reference());
						}
						bn->contexts.push_front(c);
						steps.push_front(def::step(an, bn, 1, 1,s.index+1));
						bc.push_front(c);
					} else {
						steps.push_front(def::step(an, bn, s.ai, s.bi,s.index+1));
					}
				}
				break;
			}
			}
			break;
		}
		case def::term_kind_e::structure:
		{
			def::structure* bn = static_cast<def::structure*>(b);
			switch (a->kind)
			{
			case def::term_kind_e::structure:
			{
				std::cout << "structure structure " << s.ai << " " << s.bi << std::endl;
				def::structure* an = static_cast<def::structure*>(a);
				if (callback)
				{
					if (goal)
					{
						s.ai += 1;
						s.bi += 1;
						if (s.ai == an->terms.size())
						{
							steps.pop_front();
						}
						else {
							callback = false;
						}
					}
					else {
						steps.pop_front();
					}
				}
				else {
					if (an->terms.size() == bn->terms.size())
					{
						steps.push_front(def::step(an, bn, 0, 0,s.index+1));
					}
					else {
						callback = true;
						goal = false;
						steps.pop_front();
					}
				}
				break;
			}
			default:
			{
				callback = true;
				goal = false;
				break;
			}
			}
			break;
		}
		case def::term_kind_e::atom:
		{
			def::atom* bn = static_cast<def::atom*>(b);
			switch (a->kind)
			{
			case def::term_kind_e::atom:
			{
				std::cout << "atom atom" << std::endl;
				def::atom* an = static_cast<def::atom*>(a);
				callback = true;
				goal = (an->name == bn->name);
				break;
			}
			default:
			{
				callback = true;
				goal = false;
				break;
			}
			}
			break;
		}
		case def::term_kind_e::variable:
		{
			def::variable* bn = static_cast<def::variable*>(b);
			switch (a->kind)
			{
			case def::term_kind_e::atom:
			{
				std::cout << "variable atom" << std::endl;
				def::atom* an = static_cast<def::atom*>(a);
				def::reference& bv = (*bc.front())[bn->index];
				if (bv.item)
				{
					def::term* bt = bv.item;
					switch (bt->kind)
					{
					case def::term_kind_e::atom:
					{
						callback = true;
						goal = (an->name == static_cast<def::atom*>(bt)->name);
						break;
					}
					default:
					{
						callback = true;
						goal = false;
						break;
					}
					}
				} else {
					callback = true;
					goal = true;
					bv.item = an;
				}
				break;
			}
			default:
			{
				callback = true;
				goal = false;
				break;
			}
			}
			break;
		}
		}
	}


}
