#include "Solver.h"

Solver::Solver()
{

}
Solver::~Solver()
{

}

bool Solver::solve(def::term* a, def::term* b)
{
	bindings.push_front(nullptr);
	def::structure* an = static_cast<def::structure*>(a);
	def::rule* bn = static_cast<def::rule*>(b);
	steps.push_front(new def::step(an, bn, 1, 1, 0, bindings.front()));
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
		def::step* s = steps.front();
		SetConsoleTextAttribute(hConsole, 12);
		//system("pause");
		std::cout << "#Stack size:" << steps.size() << "   Callback:" << callback << "   Goal:" << goal << "   Index:" << s->index << std::endl;
		SetConsoleTextAttribute(hConsole, 128);
		if (callback)
		{
			a = s->a;
			b = s->b;
		}
		else {
			a = s->a->terms[s->ai];
			b = s->b->terms[s->bi];
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
				std::cout << "structure rule " << s->ai << " " << s->bi << std::endl;
				def::structure* an = static_cast<def::structure*>(a);
				if (callback)
				{
					if (goal)
					{
						bool add_variant = false;
						if ((s->ai+1) < an->terms.size())
						{
							steps.push_front(new def::step(an, bn, s->ai, s->bi,s->index,s->bind));
							std::swap(steps.begin(), steps.begin()++);
							add_variant = true;
							std::cout << "swap" << s->index << std::endl;
							s = steps.front();
						}
						s->ai = 1;
						s->bi += 1;
						if (s->bi == bn->terms.size())
						{
							if (add_variant)
							{
								def::step* p = *((steps.begin()++)++);
								if (p->index == 0)
								{
									return true;
								}
								steps.push_front(p);
							} else {
								if (s->index == 0)
								{
									return true;
								}
							}
						} else {
							callback = false;
							steps.push_front(new def::step(an, bn, s->ai, s->bi, s->index + 1, bindings.front()));
						}
					} else {
						s->ai += 1;
						if (s->ai == an->terms.size())
						{
							std::cout << "step 1" << std::endl;
							while (bindings.front()!=s->bind)
							{
								std::cout << "step 1a" << std::endl;
								bindings.front()->ref->terms.back() = nullptr;
								bindings.pop_front();
							}
							if (bindings.size() != 1)
							{ 
								for (int i = 0; i < bn->context.size(); i++)
								{
									bn->context[i]->terms.pop_back();
								}
							}
							steps.pop_front();
						}
						else {
							std::cout << "bindings " << bindings.size() << std::endl;
							std::cout << "step 2" << std::endl;
							s = steps.front();
							while (bindings.front() != s->bind)
							{
								std::cout << "step 2a" << std::endl;
								bindings.front()->ref->terms.back() = nullptr;
								bindings.pop_front();
							}
							std::cout << "bindings " << bindings.size()<< std::endl;
							callback = false;
						}
					}
				}
				else {
					if (s->bi == 0)
					{
						std::cout << "context made" << s->bi << std::endl;
						for (int i = 0; i < bn->context.size(); i++)
						{
							bn->context[i]->terms.push_back(nullptr);
						}
						steps.push_front(new def::step(an, bn, 1, 1, s->index + 1, bindings.front()));
					} else {
						steps.push_front(new def::step(an, bn, s->ai, s->bi, s->index + 1, bindings.front()));
					}
				}
				std::cout << def::WriteTerm(a) << std::endl;
				std::cout << def::WriteTerm(b) << std::endl;
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
				std::cout << "structure structure " << s->ai << " " << s->bi << std::endl;
				std::cout << def::WriteTerm(a) << std::endl;
				std::cout << def::WriteTerm(b) << std::endl;
				def::structure* an = static_cast<def::structure*>(a);
				if (callback)
				{
					if (goal)
					{
						s->ai += 1;
						s->bi += 1;
						if (s->ai == an->terms.size())
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
						steps.push_front(new def::step(an, bn, 0, 0, s->index + 1, bindings.front()));
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
				std::cout << "structure default " << s->ai << " " << s->bi << std::endl;
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
				std::cout << def::WriteTerm(a) << std::endl;
				std::cout << def::WriteTerm(b) << std::endl;
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
				def::reference* br = bn->item;
				if (br->terms.back())
				{
					def::term* bt = br->terms.back();
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
					br->terms.back() = an;
					bindings.push_front(new def::binding(br));
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
		//system("pause");
	}
}
