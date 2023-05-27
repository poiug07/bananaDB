//
// Created by Glenn Salter on 29/8/2022.
//

#ifndef BANANA_DB_SERVER_H
#define BANANA_DB_SERVER_H

#include <vector>
#include <string>

class server {
public:
    // Main function of server
    int run();
    // This function splits command delimited by space and returns as vector.
    std::vector<std::string> string_split(std::string str);
};


#endif //BANANA_DB_SERVER_H
