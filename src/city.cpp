#pragma once
#include "city.h"

City::City(string name, int x_axit, int y_axis)
{
	this->name = name;
	this->x = x_axit;
	this->y = y_axis;
}

bool City::operator<(const City& city) const
{
	return y < city.y;
}

void City::print() const
{
	cout << name << " (" << x << ", " << y << ")\n";
}

