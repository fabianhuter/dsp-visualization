#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <sstream>
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
    vector<double> sine_wave;
    int numOfSamples = sampleRate * duration;
    double radiantVelocity = 2 * M_PI * frequency;
    for(int i = 0; i < numOfSamples; i++) {
        double sineValue = amplitude * sin(radiantVelocity * ((double) i / sampleRate));
        sine_wave.push_back(sineValue);
    }
    return sine_wave;
}


void functionToJSON(vector<double> function) {
    string data = "[";
    for(int i = 0; i < (int)function.size(); i++) {
        data += R"({"index": )" + std::to_string(i) + R"(, "value": )" + std::to_string(function[i]) + "}";
        if(i < function.size() -1) {
            data += ", ";
        }
    }
    data += "]";
}

void onSineWave(const shared_ptr<Session> session)
{
    const auto request = session->get_request();
    
    // Get query parameters
    int sampleRate = 400;
    int duration = 1;
    double amplitude = 1.0;
    double frequency = 50.0;

    // Parse query parameters
    const auto query_params = request->get_query_parameters();
    
    auto it = query_params.find("sampleRate");
    if (it != query_params.end()) {
        sampleRate = stoi(it->second);
    }
    it = query_params.find("duration");
    if (it != query_params.end()) {
        duration = stoi(it->second);
    }
    it = query_params.find("amplitude");
    if (it != query_params.end()) {
        amplitude = stod(it->second);
    }
    it = query_params.find("frequency");
    if (it != query_params.end()) {
        frequency = stod(it->second);
    }

    vector<double> sine_wave = generateSineWave(sampleRate, duration, amplitude, frequency);
    
    // Convert sine_wave to JSON 
    
    string data = functionToJSON(sine_wave);

    session->close(OK, data, {
        { "Access-Control-Allow-Origin", "*" },
        { "Content-Type", "application/json" },
        
        });
}








int main(const int, const char**)
{
    auto resource = make_shared<Resource>();
    resource->set_path("/sine_wave");
    resource->set_method_handler("OPTIONS", cors_options_handler);
    resource->set_method_handler("GET", onSineWave);

    auto settings = make_shared<Settings>();
    settings->set_port(8080);
    settings->set_default_header("Connection", "close");

    Service service;
    service.publish(resource);

    cout << "Starting Restbed service on port 8080..." << endl;
    service.start(settings);

    return EXIT_SUCCESS;
}
