#include "definition.h"

std::string def::WriteTerm(def::term* a)
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
				result += WriteTerm(CurrentTerm);
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
				result += WriteTerm(CurrentTerm);
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
	{
		def::reference* n = static_cast<def::variable*>(a)->item;
		if (n->terms.front())
		{
			return WriteTerm(n->terms.front());
		}
		else
		{
			return "?" + n->name;
		}
	}
	}
};