#pragma once

#include <cstdio>
#include "surface.h"
#include "Replic.h"
#include <iostream>
#include <vector>


struct Dialogue
{
	Dialogue(const std::vector<Replic>& replics) : replics(replics) {}
	virtual ~Dialogue() {}

	std::vector<Replic> replics;
};

