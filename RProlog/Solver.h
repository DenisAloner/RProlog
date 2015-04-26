#pragma once
#include "definition.h"
#include <iostream>
#include <string>
#include <Windows.h>
class Solver
{
public:

	Solver();
	~Solver();

	bool solve(def::term* a, def::term* b);

private:
	
	bool callback;
	bool goal;

	std::list<def::binding*> bindings;
	std::list<def::step*> steps;
	bool trace();
	

};

