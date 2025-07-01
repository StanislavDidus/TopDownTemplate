#pragma once

#include <cstdio>
#include "surface.h"
#include <iostream>
#include <vector>

struct Replic 
{
	Replic(const std::string& text, float time) : text(text), time(time) {}

	std::string text;
	float time;
};

