#pragma once
#include "city.h" // struct of City
#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>

class CitiesMap
{
private:
    std::set<City> citySet; // set of cities
    std::unordered_map<std::string, std::set<City>::iterator> cityMap;

public:

    // ctor of CitiesMap:
    CitiesMap();

    // ctor of CitiesMap from file:
    CitiesMap(const std::string& filename);

    // add new city
    void addCity(const std::string& name, double x_axis, double y_axis);

    // delete a city
    void deleteCity(const std::string& name);

    // add cities from file:
    void readCitiesFromFile(const std::string& filename);

    // print a city by name:
    void printCity(const std::string& name) const;

    // print all cities:
    void printAllCities() const;

    // print Nearby Cities:
    void printNearbyCities(const std::string& name, double distance) const;

    // Euclidean distance - l2 norm:
    double euclideanDistance(const City& city1, const City& city2) const;

    // Manhattan distance - l1 norm:
    double manhattanDistance(const City& city1, const City& city2) const;

    // l infinity norm:
    double infinityNorm(const City& city1, const City& city2) const;

};





