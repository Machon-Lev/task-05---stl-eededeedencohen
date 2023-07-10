#include "citiesMap.h"

int main() {
    try {
        // create a map with cities from file:
        CitiesMap map("data.txt");

        // run the program:
        map.run();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument exception: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime error exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "General exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    system("pause");
    return 0;
}
