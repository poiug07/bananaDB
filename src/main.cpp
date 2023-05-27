#include <iostream>
#include <server/server.h>

int main(int argc, char ** argv) {
    auto *s = new server();
    s->run();
}
