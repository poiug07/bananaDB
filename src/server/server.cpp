//
// Created by Glenn Salter on 29/8/2022.
//

#include "server.h"
#include "store.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <thread>

int MAX_THREADS = 3;

void server::handle_connection(int sock, Store* store) {
    char buffer[256];
    std::memset(buffer, 0, sizeof(buffer));

    while(true) {
        int n = read(sock, buffer, sizeof(buffer)-1);
        if (n < 0) {
            std::cout << "Error reading from socket" << "\n";
            close(sock);
            return;
        }

        // check if message is "quit"
        std::cout << "Received message: " << buffer << "\n";
        if (std::strcmp(buffer, "QUIT\n")==0) {
            break;
        }

        std::string command = std::string(buffer);
        std::vector<std::string> commands = string_split(command);
        std::string response = "";

        // parse command and generate response
        if (commands[0] == "GET") {
            std::pair<std::string, bool> query = store->Get(commands[1]);
            if (query.second) {
                response = "Successfully get key: " + commands[1] + " as val : " + query.first + "\n";
            } else {
                response = "Failed to get. \n";
            }
        } else if (commands[0] == "SET") {
            bool query = store->Set(commands[1], commands[2]);
            if (query) {
                response = "Successfully set key: " + commands[1] + " to val: " + commands[2] + "\n";
            } else {
                response = "Failed to set.\n";
            }
        } else if (commands[0] == "DEL") {
            bool query = store->DeleteByKey(commands[1]);
            if (query) {
                response = "Successfully deleted key: " + commands[1] + "\n";
            } else {
                response = "Failed to delete.\n";
            }
        } else if (commands[0] == "GETALL") {
            std::vector<std::string> query = store->GetAllKeys();
            response = "Keys: ";
            for (int i=0; i<query.size(); i++) {
                response += query[i];
                response += " ";
                if (i<query.size()-1) {
                    response += ",";
                }
            }
            response += "\n";
        } else if (commands[0] == "GETSET") {
            std::string query = store->GetSet(commands[1], commands[2]);
            response = "Successfully get key: " + commands[1] + " as val: " + query + "\n";
        }
        
        // send response to client
        n = write(sock, response.c_str(), response.length());
        if (n < 0) {
            std::cout << "Error writing to socket" << "\n";
            close(sock);
            return;
        }
        std::memset(buffer, 0, sizeof(buffer));
    }

    std::cout << "Closing connection" << "\n";
    close(sock);
}

std::vector<std::string> server::string_split(std::string str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    for (std::string s; iss >> s; )
        result.push_back(s);
    return result;
}

int server::run() {
    // store instance
    Store *s = new Store();

    int port = 12345;

    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // setup server address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // bind the socket to server address
    int bind_value = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (bind_value < 0) {
        perror("Could not bind");
        return 1;
    }

    // listen for incoming connections
    int listen_value = listen(sock, 1);
    if (listen_value < 0) {
        perror("Could not listen ");
        return 1;
    }

    std::cout << "Listening on port " << port << "\n";

    // accept incoming connections and handle them in threads
    std::vector<std::thread> threads;
    while (true) {
        int newsock = accept(sock, nullptr, nullptr);
        if (newsock < 0) {
            std::cout << "Error accepting connection" << "\n";
            continue;
        }
        
        std::cout << "Accepted new connection" << "\n";

        // limit the number of threads to maximum allowed
        if (threads.size() >= MAX_THREADS) {
            threads[0].join();
            threads.erase(threads.begin());
        }

        // handle new connection in a new thread
        threads.emplace_back(std::thread([&newsock, &s](){handle_connection(newsock, s);}));
    }

    // wait for all threads to finish before exiting
    for (auto& thread: threads) {
        thread.join();
    }

    close(sock);
    return 0;
}
