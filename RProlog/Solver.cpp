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

void Solver::write_step_stack()
{
	def::Console::get().set_color(6);
	std::cout << "Steps:";
	std::cout << std::endl;
	for (auto i = steps.begin(); i != steps.end(); i++)
	{
		def::Console::get().set_color(6);
		std::cout << (*i)->variation << " ";
		std::cout << (*i)->index << " " << (*i)->ai << " " << (*i)->bi << ": ";
		def::Console::get().write_term((*i)->a);
		def::Console::get().set_color(6);
		std::cout << " | ";
		def::Console::get().write_term((*i)->b);
		std::cout << std::endl;
	}
}

bool Solver::trace()
{
	def::term* a;
	def::term* b;
	goal = false;
	callback = false;
	rollback = false;
	while (true)
	{
		if (steps.empty())
		{
			return goal;
		}
		def::step* s = steps.front();
		if (rollback)
		{
			steps.pop_front();
			s = steps.front();
		}
		def::Console::get().set_color(128);
		std::cout << "#Stack size:" << steps.size() << "   Rollback:" << rollback << "   Callback:" << callback << "   Goal:" << goal << "   Index:" << s->index << std::endl;
		def::Console::get().set_color(8);
		write_step_stack();
		if (callback)
		{
			if (rollback)
			{
				a = s->a->terms[s->ai];
				b = s->b->terms[s->bi];
				if ((a->kind == def::term_kind_e::rule) && (b->kind == def::term_kind_e::structure))
				{
					def::Console::get().set_color(4);
					std::cout << "not roll" << std::endl;
				}
				else
				{
					a = s->a;
					b = s->b;
				}
			} else {
				a = s->a;
				b = s->b;
			}

		}
		else {
			a = s->a->terms[s->ai];
			b = s->b->terms[s->bi];
		}
		rollback = false;
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
						if ((s->ai + 1) < an->terms.size())
						{
							def::step* v = new def::step(an, bn, s->ai + 1, s->bi, s->index, s->bind);
							v->variation = true;
							steps.push_front(v);
							std::swap(*steps.begin(), *std::next(steps.begin()));
							add_variant = true;
							s = steps.front();
						}
						write_step_stack();
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
							}
							else {
								if (s->index == 0)
								{
									return true;
								}
							}
						}
						else {
							callback = false;
							s->bind = bindings.front();
							//steps.push_front(new def::step(an, bn, s->ai, s->bi, s->index, bindings.front()));
						}
						std::cout << "------" << std::endl;
						write_step_stack();
					}
					else {
						s->ai += 1;
						if (s->ai == an->terms.size())
						{
							std::cout << "step 1" << std::endl;
							while (bindings.front() != s->bind)
							{
								std::cout << "step 1a" << std::endl;
								bindings.front()->ref->terms.back() = nullptr;
								bindings.pop_front();
							}
							rollback = true;
							//steps.pop_front();
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
							std::cout << "bindings " << bindings.size() << std::endl;
							callback = false;
						}
					}
				}
				else {
					if (s->bi == 0)
					{
						steps.push_front(new def::step(an, bn, 1, 1, s->index + 1, bindings.front()));
					}
					else {
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
							rollback = true;
							//steps.pop_front();
						}
						else {
							callback = false;
						}
					}
					else {
						rollback = true;
						//steps.pop_front();
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
						rollback = true;
						//steps.pop_front();
					}
				}
				break;
			}
			case def::term_kind_e::rule:
			{
				std::cout << "rule structure " << s->ai << " " << s->bi << std::endl;
				def::rule* an = static_cast<def::rule*>(a);
				if (callback)
				{
					if (goal)
					{
						std::cout << "step 1" << std::endl;
						//s = steps.front();
						if (steps.size() == 1)
						{
							steps.push_front(new def::step(static_cast<def::structure*>(s->a), an, 0, 0, s->index + 1, bindings.front()));
						}
						else {
							steps.push_front(new def::step((*std::next(steps.begin()))->a, an, 0, 0, s->index + 1, bindings.front()));
						}
						write_step_stack();
					}
					else {
						if (bindings.size() != 1)
						{
							def::Console::get().set_color(4);
							std::cout << "delete bindings " << bindings.size() << std::endl;
							s = steps.front();
							while (bindings.front() != s->bind)
							{
								bindings.front()->ref->terms.back() = nullptr;
								bindings.pop_front();
							}
							for (int i = 0; i < an->context.size(); i++)
							{
								if (an->context[i]->terms.size() != 1){ an->context[i]->terms.pop_back(); }
							}
							//steps.pop_front();
						}
					}
				}
				else {
					for (int i = 0; i < an->context.size(); i++)
					{
						an->context[i]->terms.push_back(nullptr);
					}
					steps.push_front(new def::step(bn, static_cast<def::structure*>(an->terms[0]), 0, 0, s->index + 1, bindings.front()));
					write_step_stack();
					system("pause");
				}
				break;
			}
			default:
			{
				std::cout << "default structure " << s->ai << " " << s->bi << std::endl;
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
				std::cout << "atom variable" << std::endl;
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
				}
				else {
					callback = true;
					goal = true;
					br->terms.back() = an;
					def::Console::get().set_color(4);
					std::cout << "error" << std::endl;
					bindings.push_front(new def::binding(br));
				}
				break;
			}
			default:
			{
				std::cout << "default variable" << std::endl;
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
	return goal;
}
