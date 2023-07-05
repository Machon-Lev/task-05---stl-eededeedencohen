#include <algorithm>
#include <vector>
#include "citiesMap.h"


CitiesMap::CitiesMap() {
    cout << "cities Map created" << std::endl;
}

CitiesMap::CitiesMap(const std::string& filename) {
    readCitiesFromFile(filename);
}

void CitiesMap::addCity(const std::string& name, double x_axis, double y_axis)
{
    // Check if the city already exists
    if (cityMap.find(name) != cityMap.end()) {
        std::cout << "City: " << name << " already exists." << std::endl;
        return;
    }

    // Create a new city
    City city(name, x_axis, y_axis);

    // Insert the city into the set and store the iterator
    auto result = citySet.insert(city); // result is a pair: <iterator, bool>.  
    if (result.second) { // if the city was inserted
        // Insert the city into the map
        cityMap.insert({ name, result.first });
    }
    else {
        cout << "++++++++++++++++++++++++++++++" << std::endl;
        city.print();
        cout << "++++++++++++++++++++++++++++++" << std::endl;
        std::cout << "Failed to add city: " << name << std::endl;
    }
}




void CitiesMap::deleteCity(const std::string& name)
{
    // find the city in map:
    auto cityInMapIterator = cityMap.find(name); // the iterator in the map 

    // checking if the city is in the map:
    if (cityInMapIterator != cityMap.end()) // case the city is in the unordered_map structure
    {
        // copy the iterator from the map to the set:
        std::set<City>::iterator cityInSetIterator = cityInMapIterator->second;

        // erase the city from the set:
        citySet.erase(cityInSetIterator); // run time is O(1) because the iterator points to the city in the set

        // erase the city from the map:
        cityMap.erase(cityInMapIterator); // run time is O(1) for map deletion

        std::cout << "City: " << name << " deleted successfully." << std::endl;
    }
    else // case the city is not in the map:
    {
        std::cout << "City: " << name << " not found." << std::endl;
    }
}

void CitiesMap::readCitiesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Failed to open the file: " << filename << std::endl;
        return;
    }
    std::string line;
    std::string cityName;
    double x, y;
    bool readName = true; // flag to indicate if the next line is a city name or coordinates

    while (std::getline(file, line)) {
        if (readName) {
            cityName = line;
            readName = false;
        }
        else {
            char hyphenCharacter; // will containt the hyphen character

            std::stringstream ss(line); // ss is a stream that contains the line (like cin)
            ss >> x >> hyphenCharacter >> y;

            // add the city to the map if the city is not already in the map:
            if (cityMap.find(cityName) == cityMap.end()) {
                addCity(cityName, x, y);
            }
            else { 
                std::cout << "City: " << cityName << " already exists." << std::endl;
            }
            readName = true;
        }
    }

    file.close();
}




void CitiesMap::printAllCities() const {
    /*
    for (const auto& city : citySet) {
        city.print();
    }
    */
    // create an iterator to run over the set:
    std::set<City>::iterator it = citySet.begin();
    // run over the set without using loop like while or for or for_each:

}


// printCity:
void CitiesMap::printCity(const std::string& name) const {
    // find the city in map:
    auto cityInMapIterator = cityMap.find(name); // the iterator in the map 
    // checking if the city is in the map:
    if (cityInMapIterator != cityMap.end()) // case the city is in the unordered_map structure
    {
        // copy the iterator from the map to the set:
        std::set<City>::iterator cityInSetIterator = cityInMapIterator->second;
        // print the city: using the print function of the city class
        cityInSetIterator->print();
    }
    else // case the city is not in the map:
    {
        std::cout << "City: " << name << " not found." << std::endl;
    }
}

void CitiesMap::printNearbyCities(const std::string& name, double distance) const {
    // find the city in map:
    auto cityInMapIterator = cityMap.find(name); // the iterator in the map

    // checking if the city is in the map:
    if (cityInMapIterator == cityMap.end()) // case the city is in the unordered_map structure
    {
        return;
    }

    // copy the iterator from the map to the set:
    std::set<City>::iterator cityInSetIterator = cityInMapIterator->second;

    // copy the Y axis of the city:
    double cityYAxis = cityInSetIterator->y_axis;

    auto nearest_city_set = std::set<City>();

    City centerCity = *cityInSetIterator;

    auto nearest_city_oustide_range_Iterator = std::find_if_not(cityInSetIterator, citySet.end(), [this, centerCity, &nearest_city_set, cityYAxis, distance](const City& city) {
        if (this->euclideanDistance(centerCity, city) <= distance) {
            city.print();
            cout<< "Euclidean Distance: " << this->euclideanDistance(centerCity, city) << std::endl;
			nearest_city_set.insert(city);
		}
        return (city.y_axis - cityYAxis) <= distance; // return true if the city is in the range.
        });
}


// euclideanDistance implementation:
double CitiesMap::euclideanDistance(const City& city1, const City& city2) const {
    return sqrt(pow(city1.x_axis - city2.x_axis, 2) + pow(city1.y_axis - city2.y_axis, 2));
}

// manhattanDistance implementation:
double CitiesMap::manhattanDistance(const City& city1, const City& city2) const {
    return abs(city1.x_axis - city2.x_axis) + abs(city1.y_axis - city2.y_axis);
}

// infinityNorm implementation:
double CitiesMap::infinityNorm(const City& city1, const City& city2) const {
    return std::max(abs(city1.x_axis - city2.x_axis), abs(city1.y_axis - city2.y_axis));
}
