#pragma once
#include "city.h" // struct of City
#include <iostream>
using std::cin;
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>

class CitiesMap
{
private:

    // set of cities - ordered by y_axis fron the smallest to the biggest (if y_axis is equal, ordered by name)
    std::set<City> citySet;

    // unordered_map - key is city name and value is a iterator to the city in the citySet 
    std::unordered_map<std::string, std::set<City>::iterator> cityMap;

    // unordered_map - key is number of distance function and value is a pointer to the function
    std::unordered_map<int, double (CitiesMap::*)(const City&, const City&) const> distance_function_map; 


public:

    // ctor of CitiesMap:
    CitiesMap();

    // ctor of CitiesMap from file:
    CitiesMap(const std::string& filename);

    // return true if the city exists in the dataset:
    bool isCityExists(const std::string& name) const;

    // return the city by name:
    City getCity(const std::string& name) const;

    // add new city:
    void addCity(const std::string& name, double x_axis, double y_axis);

    // delete a city:
    void deleteCity(const std::string& name);

    // add cities from file:
    void readCitiesFromFile(const std::string& filename);

    // print a city details by name:
    void printCity(const std::string& name) const;

    // print all cities:
    void printAllCities() const;

    // print Nearby Cities:
    void printNearbyCities(const std::string& name, double distance, int functionNumber) const;



    //========================
    //  DISTANCE FUNCTIONS
    //========================
    
    // Euclidean distance - l2 norm:
    double euclideanDistance(const City& city1, const City& city2) const;

    // Manhattan distance - l1 norm:
    double manhattanDistance(const City& city1, const City& city2) const;

    // Infinity norm distance - l-infinity norm:
    double infinityNorm(const City& city1, const City& city2) const;

    //==========================
// User Interface Functions
//==========================
    void run();

    void print_main_menu() const;

    void add_city_user_interface();

    void delete_city_user_interface();

    void search_city_user_interface();

    void search_nearby_cities_user_interface();

    int get_option_from_user(int min, int max) const;

};























