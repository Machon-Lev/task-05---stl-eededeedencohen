#include <algorithm>
#include "citiesMap.h"
#include <exception>
#include <filesystem>


// ctor of CitiesMap:
CitiesMap::CitiesMap() {
    // inserting the distance functions to the map
    _distanceFunctionMap.insert({ 0, &CitiesMap::euclideanDistance });
    _distanceFunctionMap.insert({ 1, &CitiesMap::manhattanDistance });
    _distanceFunctionMap.insert({ 2, &CitiesMap::infinityNorm });
}

// ctor of CitiesMap from file:
CitiesMap::CitiesMap(const std::string& filename) {
    
    // read the cities from a file:
    readCitiesFromFile(filename);

    // inserting the distance functions to the map
    _distanceFunctionMap.insert({ 0, &CitiesMap::euclideanDistance });
    _distanceFunctionMap.insert({ 1, &CitiesMap::manhattanDistance });
    _distanceFunctionMap.insert({ 2, &CitiesMap::infinityNorm });
}

// return true if the city exists in the dataset:
bool CitiesMap::isCityExists(const std::string& name) const {
	return _cityMap.find(name) != _cityMap.end();
}

// using this function when we know that the city exists in the map
City CitiesMap::getCity(const std::string& name) const {
	auto cityInMapIterator = _cityMap.find(name); 
	return *cityInMapIterator->second; 
}

void CitiesMap::addCity(const std::string& name, double x_axis, double y_axis)
{
    // Check if the city already exists
    if (isCityExists(name))
    {
        std::cout << "City: " << name << " already exists." << std::endl;
        return;
    }

    // Create a new city
    City city(name, x_axis, y_axis);

    // Insert the city into the set and store the iterator
    auto result = _citySet.insert(city); // result is a pair: <iterator, bool>.  
    if (result.second) { // if the city inserted successfully
        // Insert the city into the map
        _cityMap.insert({ name, result.first });
    }
}

// print a city details by name:
void CitiesMap::deleteCity(const std::string& name)
{
    // find the city in map:
    auto cityInMapIterator = _cityMap.find(name); // the iterator in the map 

    // checking if the city is in the map:
    if (cityInMapIterator != _cityMap.end()) // case the city is in the unordered_map structure
    {
        // copy the iterator from the map to the set:
        std::set<City>::iterator cityInSetIterator = cityInMapIterator->second;

        // erase the city from the set:
        _citySet.erase(cityInSetIterator); // run time is O(1) because the iterator points to the city in the set

        // erase the city from the map:
        _cityMap.erase(cityInMapIterator); // run time is O(1) for map deletion
    }
}

void CitiesMap::readCitiesFromFile(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        throw std::invalid_argument("File path does not exist.");
    }

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open the file: " + filename);
    }

    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        throw std::runtime_error("File is empty.");
    }
    file.seekg(0, std::ios::beg);

    std::string line;
    std::string cityName;
    double x, y;
    bool readName = true; // flag to indicate if the next line is a city name or coordinates

    int line_number = 0; // to track the line number for the exception
    while (std::getline(file, line)) {
        line_number++; // increment the line number for each read line

        try {
            if (readName) {
                cityName = line;
                if (!std::isalpha(cityName[0])) {
                    throw std::invalid_argument("Invalid city name format at line " + std::to_string(line_number) + ": " + line);
                }
                readName = false;
            }
            else {
                char hyphenCharacter; // will contain the hyphen character

                std::stringstream ss(line); // ss is a stream that contains the line (like cin)
                if (!(ss >> x >> hyphenCharacter >> y)) {
                    throw std::invalid_argument("Invalid data format at line " + std::to_string(line_number) + ": " + line);
                }

                // add the city to the map if the city is not already in the map:
                if (_cityMap.find(cityName) == _cityMap.end()) {
                    addCity(cityName, x, y);
                }
                else {
                    std::cout << "City: " << cityName << " already exists." << std::endl;
                }
                readName = true;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << '\n';
            file.close();
            throw; // rethrow the exception to the caller
        }
    }

    file.close();
}





// print all the cities in the dataset:
void CitiesMap::printAllCities() const {

    std::set<City>::iterator it = _citySet.begin();
   
    int num_of_cities = std::count_if(_citySet.begin(), _citySet.end(), [&it](const City& city) {
		city.print();
		return true;
	});

    std::cout << "Number of cities: " << num_of_cities << std::endl;
}


// print the detais of a city by name:
void CitiesMap::printCity(const std::string& name) const {

    // find the city in map:
    auto cityInMapIterator = _cityMap.find(name); // the iterator in the map 

    // checking if the city is in the map:
    if (cityInMapIterator != _cityMap.end()) // case the city is in the unordered_map structure
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
    auto cityInMapIterator = _cityMap.find(name); //iterator to pair: <city name, iterator to the city in the set> 

    // case the given city is not in the map:
    if (cityInMapIterator == _cityMap.end())
    {
        return;
    }

    // comperator - will be used to sort the set of cities by distance from the center city:
    auto comparator = [this, cityInMapIterator, functionNumber](const City& city1, const City& city2) {
        double dist1 = (this->*_distanceFunctionMap.at(functionNumber))(*cityInMapIterator->second, city1);
        double dist2 = (this->*_distanceFunctionMap.at(functionNumber))(*cityInMapIterator->second, city2);
        if (dist1 == dist2)
        {
            return city1.name < city2.name;
        }
        return dist1 < dist2;
    };


    // copy iterator of the center city:
    std::set<City>::iterator centerCityIterator = cityInMapIterator->second;

    // will contain cities that are in the range ordered by distance from the center city:
      auto nearest_city_set = std::set<City, decltype(comparator)>(comparator);

    // copy of the center city:
    City centerCity = *centerCityIterator;


    //-----------------------------------------------------
    // Scaning the set from the center city to the north:
    //-----------------------------------------------------
    int north_cities_inside_range = 0;

    auto centerCityReverseIterator = std::make_reverse_iterator(centerCityIterator);
    

    auto nearest_northernCity_outside_range_Iterator = std::find_if_not(centerCityReverseIterator, _citySet.rend(), [this, centerCity, &nearest_city_set, &north_cities_inside_range, distance, functionNumber](const City& city) {
        double distanceFronCenterCity = (this->*_distanceFunctionMap.at(functionNumber))(centerCity, city);
        if (distanceFronCenterCity <= distance) {
            nearest_city_set.insert(city);
            north_cities_inside_range++;
        }
        return city.y_axis >= centerCity.y_axis - distance; // check if city's y-axis is in the north range.
        });

        
    //----------------------------------------------------
    // Scaning the set from the center city to the south:
    //----------------------------------------------------

    // copy the iterator of the center city from the map to the set:
    centerCityIterator = cityInMapIterator->second;

    // move the iterator to the next city:
    centerCityIterator = std::next(centerCityIterator);

    int south_cities_inside_range = 0;
    
    auto nearest_southernCity_outside_range_Iterator = std::find_if_not(centerCityIterator, _citySet.end(), [this, centerCity, &nearest_city_set, &south_cities_inside_range, distance, functionNumber](const City& city) {
        double distanceFronCenterCity = (this->*_distanceFunctionMap.at(functionNumber))(centerCity, city);
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
        std::cout << "Distance: " << (this->*_distanceFunctionMap.at(functionNumber))(centerCity, city) << std::endl;
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



//============================================
// User Interface Functions - Implementation:
//============================================

// run the program:
void CitiesMap::run()
{
    int option;
    do
    {   // clear the console:
        system("cls");

        print_main_menu();
        option = get_option_from_user(0, 4);
        switch (option)
        {
        case 1:
            add_city_user_interface();
            break;
        case 2:
            delete_city_user_interface();
            break;
        case 3:
            search_city_user_interface();
            break;
        case 4:
            search_nearby_cities_user_interface();
            break;
        case 0:
            return;
        default:
            break;
        }
    } while (option != 0);
}

// print main menu:
void CitiesMap::print_main_menu() const
{
    std::cout << "Please choose one of the following options. than, press enter." << std::endl;
    std::cout << "1. Add a city" << std::endl;
    std::cout << "2. Delete a city" << std::endl;
    std::cout << "3. Search for a city" << std::endl;
    std::cout << "4. Search for nearby cities" << std::endl << std::endl;
    std::cout << "0. Exit" << std::endl;
}

// get option from user:
int CitiesMap::get_option_from_user(int min, int max) const
{
    int option;
    std::cin >> option;
    if (option < min || option > max)
    {
        return -1; // incalid option
    }
    return option;
}

// add city user interface:
void CitiesMap::add_city_user_interface()
{
    std::string name;
    std::cout << "Please enter the city name, then press enter." << std::endl;

    std::cin.ignore(); // ignore the enter from the previous input

    std::getline(std::cin, name); // get the name from the user

    if (isCityExists(name))
    {
        std::cout << name << " already exists in the dataset." << std::endl;

        // get the enter from the user to return from the function
        std::cout << "Press enter to return to main menu." << std::endl;
        std::cin.ignore(); // ignore the enter from the previous input
        std::cin.get();

        return;
    }

    double x_axis = -1, y_axis = -1;
    std::cout << "Please enter the city coordinates (positive numbers separated by space), then press enter." << std::endl;

    // Ignore all the spaces and tabs before the input
    std::cin >> x_axis;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Invalid input for x-axis coordinate");
    }

    std::cin >> y_axis;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Invalid input for y-axis coordinate");
    }

    while (x_axis < 0 || y_axis < 0)
    {
        std::cout << "Invalid coordinates. Please enter the city coordinates (positive numbers separated by space), then press enter." << std::endl;
        std::cin >> x_axis >> y_axis;
    }


    addCity(name, x_axis, y_axis);

    std::cout << "The city '" << name << "' was added successfully." << std::endl;

    // pressing enter to return to main menu:
    std::cout << "Press enter to return to main menu." << std::endl;

    std::cin.ignore(); // ignore the enter from the previous input
    std::cin.get(); // get the enter from the user
    return;
}

// delete city user interface:
void CitiesMap::delete_city_user_interface()
{
    std::string name;
    std::cout << "Please enter the city name to delete. than, press enter." << std::endl;
    std::cin.ignore(); // ignore the enter from the previous input
    std::getline(std::cin, name); // get the name from the user
    if (!isCityExists(name))
    {
        std::cout << name << " does not exists in the dataset." << std::endl;
        // get the enter from the user to return from the function
        std::cout << "Press enter to return to main menu." << std::endl;
        std::cin.get();
        return;
    }
    deleteCity(name);
    std::cout << "The city '" << name << "' was deleted successfully." << std::endl;
    // pressing enter to return to main menu:
    std::cout << "Press enter to return to main menu." << std::endl;
    std::cin.get(); // get the enter from the user
    return;
}

// search for city user interface:
void CitiesMap::search_city_user_interface()
{
    std::string name;
    std::cout << "Please enter the city name to search. than, press enter." << std::endl;
    std::cin.ignore(); // ignore the enter from the previous input
    std::getline(std::cin, name); // get the name from the user
    if (!isCityExists(name))
    {
        std::cout << name << " does not exists in the dataset." << std::endl;
        // get the enter from the user to return from the function
        std::cout << "Press enter to return to main menu." << std::endl;
        std::cin.get();
        return;
    }
    this->printCity(name);
    // pressing enter to return to main menu:
    std::cout << "Press enter to return to main menu." << std::endl;
    std::cin.get(); // get the enter from the user
    return;
}

// search for nearby cities user interface:
#include <exception>
#include <limits>

void CitiesMap::search_nearby_cities_user_interface()
{
    std::string name;
    std::cout << "Please enter the city name. then, press enter." << std::endl;

    // get the name from the user
    std::cin.ignore();
    std::getline(std::cin, name);

    // while the city does not exists in the dataset:
    while (!isCityExists(name))
    {
        if (name == "0")
        {
            return;
        }
        std::cout << name << " does not exist in the dataset." << std::endl;
        std::cout << "Please enter the city name. then, press enter." << std::endl;
        std::getline(std::cin, name);
    }

    // get the radius from the user
    double radius;
    std::cout << "Please enter the radius. then, press enter." << std::endl;
    std::cin >> radius;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Invalid input for radius");
    }

    // while the radius is not positive:
    while (radius <= 0)
    {
        std::cout << "Invalid radius. Please enter the radius. then, press enter." << std::endl;
        std::cin >> radius;
    }

    // get the distance type from the user
    int distance_option;
    std::cout << "Please choose the distance type. then, press enter." << std::endl;
    std::cout << "0. Euclidean distance" << std::endl;
    std::cout << "1. Manhattan distance" << std::endl;
    std::cout << "2. Chebyshev distance" << std::endl;

    distance_option = get_option_from_user(0, 2); // input between 0 to 2

    // while the distance type is not valid:
    while (distance_option == -1)
    {
        std::cout << "Invalid distance type. Please choose the distance type. then, press enter." << std::endl;
        std::cout << "0. Euclidean distance" << std::endl;
        std::cout << "1. Manhattan distance" << std::endl;
        std::cout << "2. Chebyshev distance" << std::endl;
        distance_option = get_option_from_user(0, 2); // input between 0 to 2
    }

    // print the nearby cities
    this->printNearbyCities(name, radius, distance_option);

    // pressing enter to return to main menu:
    std::cout << "Press enter to return to main menu." << std::endl;
    std::cin.ignore(); // ignore the enter from the previous input
    std::cin.get(); // get the enter from the user
    return;
}





