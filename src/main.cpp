#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>
#include <math.h>
#include <cmath>
using namespace std;
using namespace restbed;


void cors_options_handler(const shared_ptr<Session> session)
{
    const auto request = session->get_request();
    
    // Add CORS headers
    session->close(200, "", {
        { "Access-Control-Allow-Origin", "*" },  // Allow all origins. In production, you might want to restrict this to a specific domain.
        { "Access-Control-Allow-Methods", "GET, POST, OPTIONS" }, // Methods allowed
        { "Access-Control-Allow-Headers", "Content-Type, Accept" } // Headers allowed
    });
}


vector<double> generateSineWave(int sampleRate, int duration, double amplitude, double frequency) {
    vector<double> sine_wave; // number of total samples
    int numOfSamples = sampleRate * duration;
    double radiantVelocity = 2 * M_PI * frequency;
    for(int i = 0; i < sampleRate * duration; i++) {
        double sineValue = amplitude * sin(radiantVelocity * ((double) i / sampleRate));
        sine_wave.push_back(sineValue);
    }
    return sine_wave;
}

void make_sound(const shared_ptr<Session> session)
{
    const auto request = session->get_request();

    vector<double> sine_wave = generateSineWave(400, 1, 1, 50);
    string data = "[";
    for(int i = 0; i < (int)sine_wave.size(); i++) {

        data += R"({"index": )" + std::to_string(i) + R"(, "value": )" + std::to_string(sine_wave[i]) + "}";
        if(i < sine_wave.size() -1) {
            data += ", ";
        }
    }
    data += "]";




    int numOfChars = data.length();


    session->close(OK, data, {
        { "Access-Control-Allow-Origin", "*" },
        { "Content-Type", "application/json" },
        
        });
}








int main(const int, const char**)
{
    auto resource = make_shared<Resource>();
    resource->set_path("/hello");
    resource->set_method_handler("OPTIONS", cors_options_handler);
    resource->set_method_handler("GET", make_sound);

    auto settings = make_shared<Settings>();
    settings->set_port(8080);
    settings->set_default_header("Connection", "close");

    Service service;
    service.publish(resource);

    cout << "Starting Restbed service on port 8080..." << endl;
    service.start(settings);

    return EXIT_SUCCESS;
}