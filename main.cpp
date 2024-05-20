#include "App.h"
#include <cpr/cpr.h>
#include <iostream>

int main() {
    uWS::SSLApp({
        .cert_file_name = "cert.pem",
        .key_file_name = "key.pem"
    }).get("/p", [](auto *res, auto *req) {
        
    }).listen(443, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening on port " << 443 << std::endl;
        } else {
            std::cout << "Failed to load certs or to bind to port" << std::endl;
        }
    })
    return 0;
}
