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

	std::list<def::context*> ac;
	std::list<def::context*> bc;
	std::list<def::step> steps;
	bool trace();
	

};

