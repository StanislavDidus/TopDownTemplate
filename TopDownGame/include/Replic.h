#pragma once

#include <cstdio>
#include "surface.h"
#include <iostream>
#include <vector>
#include <functional>

struct Replic 
{
	Replic(const std::string& text, float time, const std::function<void()>& func = nullptr) : text(text), time(time), func(func) {}

	std::string text;
	float time;
	std::function<void()> func;
};

