#include <string>
using std::string;
#include <iostream>
using std::cout;


struct City
{
    std::string name;
    double x_axis;
    double y_axis;

	City(const std::string& name, double x_axis, double y_axis);

    // Comparison operator for ordering cities based on their names
	bool operator<(const City& other) const;

    // Print function to display the city information
	void print() const;

    // print city name:
    void printName() const;
};
