#include "Parser.h"


Parser::Parser()
{
}


Parser::~Parser()
{
}

unsigned int Parser::parse_from_pos(def::terms* t, unsigned int s, def::rule* r)
{
	int w = s;
	for (unsigned int i = s; i<m_text.length(); i++)
	{
		switch (m_text[i])
		{
		case '[':
			switch (m_text[i + 1])
			{
			case '[':
			{
				def::rule* n = new def::rule();
				n->kind = def::term_kind_e::rule;
				t->push_back(n);
				i = parse_from_pos(&n->terms, i + 1, n);
				w = i + 1;
				break;
			}
			default:
			{
				def::structure* n = new def::structure;
				n->kind = def::term_kind_e::structure;
				t->push_back(n);
				i = parse_from_pos(&n->terms, i + 1, r);
				w = i + 1;
				break;
			}
			}
			break;
		case ']':case ' ':
			if (i - w>0)
			{
				switch (m_text[w])
				{
				case '?':
				{
					if (r != nullptr)
					{
						bool IsNew = true;
						std::string name = m_text.substr(w + 1, i - w - 1);
						for (int i = 0; i<r->context.size(); i++)
						{
							if (r->context[i]->name == name)
							{
								def::variable* n = new def::variable();
								n->kind = def::term_kind_e::variable;
								n->item = r->context[i];
								t->push_back(n);
								IsNew = false;
								break;
							}
						}
						if (IsNew)
						{
							def::reference* ref = new def::reference(m_text.substr(w + 1, i - w - 1));
							r->context.push_back(ref);
							def::variable* n = new def::variable();
							n->kind = def::term_kind_e::variable;
							n->item = ref;
							t->push_back(n);
							break;
						}
					}
					break;
				}
				default:
				{
					def::atom* n = new def::atom();
					n->kind = def::term_kind_e::atom;
					n->name = m_text.substr(w, i - w);
					t->push_back(n);
					break;
				}
				}
			}
			if (m_text[i] == ' ')
			{
				w = i + 1;
			}
			else {
				return i;
			}
			break;
		}
	}
	return 0;
}

def::term* Parser::parse(std::string text)
{
	m_text = text;
	def::term* result = nullptr;
	if (text[1] == '[')
	{
		def::rule* n = new def::rule();
		n->kind = def::term_kind_e::rule;
		parse_from_pos(&n->terms, 1, n);
		result = n;
	}
	else
	{
		def::structure* n = new def::structure;
		n->kind = def::term_kind_e::structure;
		parse_from_pos(&n->terms, 1, nullptr);
		result = n;
	}
	return result;
}