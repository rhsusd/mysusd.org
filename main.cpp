#include "App.h"
#include <cpr/cpr.h>
#include <iostream>

int main() {
    uWS::App().get("/p", [](uWS::HttpResponse<false> *res, uWS::HttpRequest *req) {
        cpr::Session session;
        std::string chunks;
        res->onData([&chunks, &session](std::string_view chunk, bool isLast) {
            chunks += std::string{chunk};
            if (isLast) {
                session.SetBody(cpr::Body{chunks});
            }
        });
        const std::string url = std::string{req->getQuery("u")};
        session.SetUrl(url);
        for (auto it = req->begin(); it != req->end(); it = req->begin()) {
            const auto [header] = it;
            session.SetHeader(cpr::Header{header->key, header->value});
        }
        const std::string method = std::string{req->getMethod()};

        cpr::AsyncResponse cprRes = cpr::AsyncResponse{std::future<cpr::Response>{}};
        if (method == "GET") {
            cprRes = session.GetAsync();
        } else if (method == "POST") {
            cprRes = session.PostAsync();
        } else if (method == "PUT") {
            cprRes = session.PutAsync();
        } else if (method == "DELETE") {
            cprRes = session.DeleteAsync();
        } else if (method == "PATCH") {
            cprRes = session.PatchAsync();
        } else if (method == "HEAD") {
            cprRes = session.HeadAsync();
        } else if (method == "OPTIONS") {
            cprRes = session.OptionsAsync();
        } else {
            res->writeStatus("405 Method Not Allowed")->end();
        }

        res->cork([&cprRes, &res]() {
            cprRes.wait();
            const cpr::Response &response = cprRes.get();
            for (auto &[key, value] : response.header) {
                res->writeHeader(key, value);
            };
            res->writeStatus(std::to_string(response.status_code))->end(response.text);
        });
    }).listen(80, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening on port " << 443 << std::endl;
        } else {
            std::cout << "Failed to load certs or to bind to port" << std::endl;
        }
    });
    return 0;
}
