#include "citiesMap.h"

int main() {
    CitiesMap map("data.txt");

    //print city:
    map.printCity("Gifford, IL");

    cout << "================================" << std::endl;
    cout << "================================" << std::endl;

    map.printNearbyCities("Gifford, IL", 3);

    system("pause");
    return 0;
}