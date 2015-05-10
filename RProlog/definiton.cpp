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

def::Console::Console()
{
	m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD  size;
	size.X = 160;
	size.Y = 1001;
	SetConsoleScreenBufferSize(m_handle, size);
	_CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_handle, &info);
	SMALL_RECT windowMaxSize = { 0, 0, info.dwMaximumWindowSize.X - 1, 40 };
	SetConsoleWindowInfo(m_handle, TRUE, &windowMaxSize);
}

void def::Console::set_color(int i)
{
	SetConsoleTextAttribute(m_handle, i);
}

void def::Console::write_term(def::term* a)
{
	switch (a->kind)
	{
	case def::term_kind_e::atom:
	{
		SetConsoleTextAttribute(m_handle, m_term_color);
		std::cout << static_cast<def::atom*>(a)->name;
		return;
	}
	case def::term_kind_e::structure:
	{
		SetConsoleTextAttribute(m_handle, m_bracket_color);
		std::cout << "[";
		if (!static_cast<def::structure*>(a)->terms.empty())
		{
			for (auto CurrentTerm : static_cast<def::structure*>(a)->terms)
			{
				write_term(CurrentTerm);
				if (CurrentTerm != static_cast<def::structure*>(a)->terms.back())
				{
					std::cout << " ";
				}
			}
		}
		SetConsoleTextAttribute(m_handle, m_bracket_color);
		std::cout << "]";
		return;
	}
	case def::term_kind_e::rule:
	{
		SetConsoleTextAttribute(m_handle, m_bracket_color);
		std::cout << "[";
		if (!static_cast<def::rule*>(a)->terms.empty())
		{
			for (auto CurrentTerm : static_cast<def::rule*>(a)->terms)
			{
				write_term(CurrentTerm);
				if (CurrentTerm != static_cast<def::rule*>(a)->terms.back())
				{
					std::cout << " ";
				}
			}
		}
		SetConsoleTextAttribute(m_handle, m_bracket_color);
		std::cout << "]";
		return;
	}
	case def::term_kind_e::variable:
	{
		def::reference* n = static_cast<def::variable*>(a)->item;
		if (n->terms.back())
		{
			write_term(n->terms.back());
		}
		else
		{
			SetConsoleTextAttribute(m_handle, m_variable_color);
			std::cout<< "?" << n->name;
		}
	}
	}
};