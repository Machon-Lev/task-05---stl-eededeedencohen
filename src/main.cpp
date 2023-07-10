#include "citiesMap.h"

int main() {
    CitiesMap map("data.txt");

    // map.printNearbyCities("Gifford, IL", 3, 2);
    map.printAllCities();


    system("pause");
    return 0;
}