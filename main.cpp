#include "App.h"
#include <cpr/cpr.h>
#include <iostream>

int main() {
    uWS::App().get("/p", [](auto *res, auto *req) {
        res->end();
    }).listen(80, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening on port " << 443 << std::endl;
        } else {
            std::cout << "Failed to load certs or to bind to port" << std::endl;
        }
    });
    return 0;
}
