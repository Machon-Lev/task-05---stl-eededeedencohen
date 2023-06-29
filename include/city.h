#include <string>
using std::string;
#include <iostream>
using std::cout;

struct City
{
	std::string name;
	int x;
	int y;

	bool operator<(const City& city) const;
	void print() const;
	// ctor:
	City(string name, int x_axit, int y_axis);

};

