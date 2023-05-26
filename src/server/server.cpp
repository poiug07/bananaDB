#include <iostream>
#include "store.h"

void hello() {
    std::cout << "hello from server" << std::endl;
    helloFromDB();
}