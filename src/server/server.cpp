//
// Created by Glenn Salter on 29/8/2022.
//

#include "server.h"
#include "store.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>

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

    int port = 5000;

    // 1 socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2 bind
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    int bind_value = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (bind_value < 0) {
        perror("Could not bind");
        return 1;
    }

    // 3 listen
    int listen_value = listen(sock, 1);
    if (listen_value < 0) {
        perror("Could not listen ");
        return 1;
    }

    // 4 accept
    struct sockaddr_in remote_address;
    memset(&remote_address, 0, sizeof(remote_address));
    socklen_t remote_addrlen = sizeof(address);

    while (true) {
        std::cout << "Waiting for new connection" << std::endl;
        int client_socket = accept(sock, (struct sockaddr *) &remote_address, &remote_addrlen);
        std::string client_ip = inet_ntoa(remote_address.sin_addr);
        int remote_port = ntohs(remote_address.sin_port);
        std::cout << "Accepted new client @ " << client_ip << ":" << remote_port << std::endl;
        std::string initial = "Hello client at " + client_ip + ":" + std::to_string(remote_port) + "\n";
        send(client_socket, initial.c_str(), initial.length(), 0);

        int BUFFLEN = 1024;
        char buffer[BUFFLEN];

        while (true) {
            memset(buffer, 0, BUFFLEN);
            // 5 recv
            int bytes_received = recv(client_socket, buffer, BUFFLEN - 1, 0);
            if (bytes_received < 0) {
                perror("Could not receive");
                return 1;
            }
            if (bytes_received == 0) {
                std::cout << "Client at " << client_ip << " : " << remote_port << " has disconnected" << std::endl;
                break;
            }
            if (buffer[bytes_received - 1] == '\n') {
                buffer[bytes_received - 1] = 0;
            }
            std::cout << "Client message: \"" << buffer << "\"" << std::endl;

            std::string command = std::string(buffer);
            std::vector<std::string> commands = string_split(command);
            std::string response = "";

            // 6 parse response
            if (commands[0] == "GET") {
                std::pair<std::string, bool> query = s->Get(commands[1]);
                if (query.second) {
                    response = "Successfully get key: " + commands[1] + " as val : " + query.first + "\n";
                } else {
                    response = "Failed to get. \n";
                }
            } else if (commands[0] == "SET") {
                bool query = s->Set(commands[1], commands[2]);
                if (query) {
                    response = "Successfully set key: " + commands[1] + " to val: " + commands[2] + "\n";
                } else {
                    response = "Failed to set.\n";
                }
            } else if (commands[0] == "DEL") {
                bool query = s->DeleteByKey(commands[1]);
                if (query) {
                    response = "Successfully deleted key: " + commands[1] + "\n";
                } else {
                    response = "Failed to delete.\n";
                }
            } else if (commands[0] == "GETALL") {
                std::vector<std::string> query = s->GetAllKeys();
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
                std::string query = s->GetSet(commands[1], commands[2]);
                response = "Successfully get key: " + commands[1] + " as val: " + query + "\n";
            }

            // 7 send response
            int bytes_sent = send(client_socket, response.c_str(), response.length(), 0);
            if (bytes_sent < 0) {
                perror("Could not send back");
                return 1;
            }
        }
        std::cout << "Shutting down socket: " << std::endl;
        shutdown(client_socket, SHUT_RDWR);
    }
}
