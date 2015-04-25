#pragma once
#include <iostream>
#include <string>
#include "definition.h"

class Parser
{
public:

	Parser();
	~Parser();

	def::term* parse(std::string text);

private:
	
	std::string m_text;

	unsigned int parse_from_pos(def::terms* t, unsigned int s, def::rule* r);
};

