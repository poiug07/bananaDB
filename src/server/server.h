//
// Created by Glenn Salter on 29/8/2022.
//

#ifndef BANANA_DB_SERVER_H
#define BANANA_DB_SERVER_H

#include "store.h"

#include <vector>
#include <string>

class server {
public:
    // Main function of server.
    int run();
    // This function handles parsing commands and sending back response for one connection.
    static void handle_connection(int sock, Store* store);
    // This function splits command delimited by space and returns as vector.
    static std::vector<std::string> string_split(std::string str);
};


#endif //BANANA_DB_SERVER_H
