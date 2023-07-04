#include "city.h"

City::City(const std::string& name, double x_axis, double y_axis)
    : name(name), x_axis(x_axis), y_axis(y_axis) {}

bool City::operator<(const City& other) const {
    if (y_axis == other.y_axis) {
        return name < other.name;
    }
    return y_axis < other.y_axis;
}


void City::print() const {
    std::cout << "City: " << name << ", X-Axis: " << x_axis << ", Y-Axis: " << y_axis << std::endl;
}
