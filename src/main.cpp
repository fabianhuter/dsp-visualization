#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>

using namespace std;
using namespace restbed;

void get_method_handler(const shared_ptr<Session> session)
{
    const auto request = session->get_request();
    session->close(OK, "Hello from Restbed!", {{"Content-Length", "19"}});
}

int main(const int, const char**)
{
    auto resource = make_shared<Resource>();
    resource->set_path("/hello");
    resource->set_method_handler("GET", get_method_handler);

    auto settings = make_shared<Settings>();
    settings->set_port(8080);
    settings->set_default_header("Connection", "close");

    Service service;
    service.publish(resource);

    cout << "Starting Restbed service on port 8080..." << endl;
    service.start(settings);

    return EXIT_SUCCESS;
}