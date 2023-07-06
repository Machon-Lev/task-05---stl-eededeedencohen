#include <algorithm>
#include <vector>
#include "citiesMap.h"


CitiesMap::CitiesMap() {
    functionMap.insert({ 0, &CitiesMap::euclideanDistance });
    functionMap.insert({ 1, &CitiesMap::manhattanDistance });
    functionMap.insert({ 2, &CitiesMap::infinityNorm });
}

CitiesMap::CitiesMap(const std::string& filename) {
    readCitiesFromFile(filename);
        functionMap.insert({ 0, &CitiesMap::euclideanDistance });
    functionMap.insert({ 1, &CitiesMap::manhattanDistance });
    functionMap.insert({ 2, &CitiesMap::infinityNorm });
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



//==================================
// PRINT NEARBY CITIES FUNCTION
//==================================
void CitiesMap::printNearbyCities(const std::string& name, double distance, int functionNumber) const {

    // find the city in map:
    auto cityInMapIterator = cityMap.find(name); //iterator to pair: <city name, iterator to the city in the set> 

    // case the given city is not in the map:
    if (cityInMapIterator == cityMap.end())
    {
        return;
    }




    // comperator: compare by Euclidean Distance from the center city:
    auto comparator = [this, cityInMapIterator, functionNumber](const City& city1, const City& city2) {
        double dist1 = (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city1);
        double dist2 = (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city2);
        if (dist1 == dist2)
        {
            return city1.name < city2.name;
        }
        return dist1 < dist2;
    };


    // copy iterator of the center city:
    std::set<City>::iterator centerCityIterator = cityInMapIterator->second;

    // will contain cities that are in the range:
      auto nearest_city_set = std::set<City, decltype(comparator)>(comparator);


    // copy of the center city:
    City centerCity = *centerCityIterator;


    //---------------------------------------------------
    // * Scaning the set from the center city to the north:
    //   form the center city to the begining of the set.
    // 
    // * breack when gettind to the first city 
    //   that the y axis is not in the range.
    //---------------------------------------------------
    int north_cities_inside_range = 0;

    auto centerCityReverseIterator = std::make_reverse_iterator(centerCityIterator);
    

    auto nearest_northernCity_outside_range_Iterator = std::find_if_not(centerCityReverseIterator, citySet.rend(), [this, centerCity, &nearest_city_set, &north_cities_inside_range, distance, functionNumber](const City& city) {
        double dist = (this->*functionMap.at(functionNumber))(centerCity, city);
        if (dist <= distance) {
            nearest_city_set.insert(city);
            north_cities_inside_range++;
        }
        return city.y_axis >= centerCity.y_axis - distance; // check if city's y-axis is in the north range.
        });

        
    //---------------------------------------------------
    // * Scaning the set from the center city to the south:
    //   form the center city to the end of the set.
    //
    // * breack when gettind to the first city
    //   that the y axis is not in the range.
    //---------------------------------------------------
    
    // copy the iterator of the center city from the map to the set:
    centerCityIterator = cityInMapIterator->second;

    // move the iterator to the next city:
    centerCityIterator = std::next(centerCityIterator);

    int south_cities_inside_range = 0;
    
    auto nearest_southernCity_outside_range_Iterator = std::find_if_not(centerCityIterator, citySet.end(), [this, centerCity, &nearest_city_set, &south_cities_inside_range, distance, functionNumber](const City& city) {
        double distanceFronCenterCity = (this->*functionMap.at(functionNumber))(centerCity, city);
        if (distanceFronCenterCity <= distance) {
            nearest_city_set.insert(city);
            south_cities_inside_range++;
        }
        return city.y_axis <= centerCity.y_axis + distance; // check if city's y-axis is in the south range.
        });


    // print the number of cities in the range:
    std::cout << nearest_city_set.size() <<  " city/cities found in the given radius." << std::endl;

    // cities north:
    std::cout << north_cities_inside_range << " cities are to the north of the selected city." << std::endl;


    // print all the cities in the set:
    std::cout << "City list:" << std::endl;
    int nearCities = std::count_if(nearest_city_set.cbegin(), nearest_city_set.cend(), [this, centerCity, &nearest_city_set, distance, functionNumber](const City& city) {
        city.printName();
        std::cout << "Distance: " << (this->*functionMap.at(functionNumber))(centerCity, city) << std::endl;
        return true;
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


/*
void CitiesMap::printNearbyCities(const std::string& name, double distance, int functionNumber) const {

    // find the city in map:
    auto cityInMapIterator = cityMap.find(name);

    // case the given city is not in the map:
    if (cityInMapIterator == cityMap.end())
    {
        return;
    }

    // comparator: compare by Distance from the center city:
    auto comparator = [this, cityInMapIterator, functionNumber](const City& city1, const City& city2) {
        double dist1 = (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city1);
        double dist2 = (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city2);
        if (dist1 == dist2)
        {
            return city1.name < city2.name;
        }
        return dist1 < dist2;
    };

    auto nearest_city_set = std::set<City, decltype(comparator)>(comparator);

    int north_cities_inside_range = 0;
    auto centerCityReverseIterator = std::make_reverse_iterator(cityInMapIterator->second);

    auto nearest_northernCity_outside_range_Iterator = std::find_if_not(centerCityReverseIterator, citySet.rend(), [this, &nearest_city_set, &north_cities_inside_range, distance, functionNumber](const City& city) {
        double dist = (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city);
        if (dist <= distance) {
            nearest_city_set.insert(city);
            north_cities_inside_range++;
        }
        return dist <= distance;
    });

    int south_cities_inside_range = 0;
    auto centerCityIterator = std::next(cityInMapIterator->second);

    auto nearest_southernCity_outside_range_Iterator = std::find_if_not(centerCityIterator, citySet.end(), [this, &nearest_city_set, &south_cities_inside_range, distance, functionNumber](const City& city) {
        double dist = (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city);
        if (dist <= distance) {
            nearest_city_set.insert(city);
            south_cities_inside_range++;
        }
        return dist <= distance;
    });

    std::cout << nearest_city_set.size() <<  " city/cities found in the given radius." << std::endl;
    std::cout << north_cities_inside_range << " cities are to the north of the selected city." << std::endl;

    std::cout << "City list:" << std::endl;
    std::for_each(nearest_city_set.cbegin(), nearest_city_set.cend(), [this, functionNumber](const City& city) {
        city.printName();
        std::cout << "Distance: " << (this->*functionMap.at(functionNumber))(*cityInMapIterator->second, city) << std::endl;
    });

}

*/