#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "DownloadThread.h"
#include "httplib.h"
#include <iostream>
#include "nlohmann/json.hpp"
#include "City.h"
#include <chrono>
#include <thread>
#include <vector>
#include <ctime>
#include <iostream>
#include <map>


nlohmann::json DownloadThread::callOpenAI(const std::string& user_prompt, int max_tokens) {
    const std::string api_endpoint = "api.openai.com";
    const std::string api_path = "/v1/chat/completions";
    const std::string api_key = keys["openai"];

    const std::string system_prompt = "You are an expert travel advisor writing for weather and traveling sites based on the current weather. assume that the user cant check any other source than your response when answering. dont advise the user to check the forcast";

    nlohmann::json request_body = {
        {"model", "gpt-4o-mini"},
        {"messages", {
            {{"role", "system"}, {"content", system_prompt}},
            {{"role", "user"}, {"content", user_prompt}}
        }},
        {"max_tokens", max_tokens}
    };
    httplib::SSLClient client(api_endpoint);

    httplib::Headers headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + api_key}
    };

    auto res = client.Post(api_path.c_str(), headers, request_body.dump(), "application/json");

    if (res && res->status == 200) {
        return nlohmann::json::parse(res->body);
    }
    else {
        std::cerr << "OpenAI API Error: " << (res ? res->status : -1) << " - " << (res ? res->body : "No response") << std::endl;
        return nlohmann::json(); // Return an empty JSON object on error
    }
}

nlohmann::json DownloadThread::callWeatherAPI(const std::string& city) {
    const std::string api_endpoint = "https://api.openweathermap.org";
    const std::string api_key = keys["weather"]; // Assume API key is stored in the keys map
    std::string url = "/data/2.5/weather?q=" + city + "&appid=" + api_key + "&units=metric";

    httplib::Client cli(api_endpoint);

    auto res = cli.Get(url.c_str());

    if (res && res->status == 200) {
        return nlohmann::json::parse(res->body);
    }
    else {
        std::cerr << "Weather API Error: " << (res ? res->status : -1) << " - " << (res ? res->body : "No response") << std::endl;
        return nlohmann::json(); // Return an empty JSON object on error
    }
}

void DownloadThread::operator()(CommonObjects* common)
{
    getKeys();
    int counter = 0;
    while (true) {
        if (common->getLocation == true)
        {
            getCurentLocation(common);
        }
        if (common->start_download == true)
            getCityTemp(*common);
        if (common->fetch_favorites == true)
            getFavCitiesTemp(common);
        if (common->quetion != -1)
            followupQuetion(common);

        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        counter++;
        if (counter == 600)
        {
            counter = 0;
            common->fetch_favorites = true;
            common->start_download = true;
        }
     
    }
}

void DownloadThread::getFavCitiesTemp(CommonObjects* common)
{
    for (int i = 0; i < common->fav_cities.size(); i++)
    {
        nlohmann::json weather_json = callWeatherAPI(common->fav_cities[i].name);

        if (!weather_json.empty()) {
            common->fav_cities[i].temperature = weather_json["main"]["temp"];
            common->fav_cities[i].description = weather_json["weather"][0]["main"];
            std::cout << weather_json.dump(4) << "\n";
        }
        else {
            std::cout << "Error retrieving weather data for " << common->fav_cities[i].name << "\n";
        }
    }
    common->fetch_favorites = false;
}

void DownloadThread::getCityTemp(CommonObjects& common)
{
    nlohmann::json weather_json = callWeatherAPI(common.city);

    if (!weather_json.empty()) {
        common.cityData.name = weather_json["name"];
        common.cityData.temperature = weather_json["main"]["temp"];
        common.cityData.description = weather_json["weather"][0]["description"];
        std::cout << weather_json.dump(4) << "\n";
        getLocationDetails(&common);
        getForcast(&common);
        common.download_success = true;
    }
    else {
        common.download_success = false;
    }
    common.start_download = false;
}

void DownloadThread::getKeys()
{
    std::ifstream file("api_keys.txt");
    std::string line;
    std::string name;
    std::string key;

    if (file.is_open())
    {
        while (std::getline(file, name) && std::getline(file, key))
        {
            keys.emplace(name, key);
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file" << std::endl;
    }
}

void DownloadThread::getLocationDetails(CommonObjects* common)
{
    std::string user_prompt = "Give the current weather and travel tips for " + common->city + ". Limit your response to 150 letter or fewer. Do not mention the current temperature. Provide the information directly, without introductory phrases or numbered lists.";

    nlohmann::json response_json = callOpenAI(user_prompt, 30);

    if (!response_json.empty()) {
        common->review = response_json["choices"][0]["message"]["content"];
        createFollowUpQuetion(common);
        std::cout << "Response: " << response_json.dump(4) << std::endl;
    }
    else {
        std::cout << "Error: Empty response from OpenAI" << std::endl;
        common->review = "No review available";
        common->questions.clear();
    }
}

void DownloadThread::followupQuetion(CommonObjects* common) {
    std::string user_prompt = "Respond to the user's question based on the city that the user is interested in. Make sure your response isn't over 30 words and is logically correct: " + common->questions[common->quetion] + ". City: " + common->city;

    nlohmann::json response_json = callOpenAI(user_prompt, 35);

    if (!response_json.empty()) {
        common->review = response_json["choices"][0]["message"]["content"];
        common->quetion = -1;
        createFollowUpQuetion(common);
        std::cout << "Response: " << response_json.dump(4) << std::endl;
    }
    else {
		std::cout << "Error: Empty response from OpenAI" << std::endl;
		common->review = "No answer avalible";
		common->questions.clear();
	}
}

void DownloadThread::createFollowUpQuetion(CommonObjects* common) {
    std::string user_prompt = "Write 4 follow-up questions about the review: " + common->review + ". Each question should consist of 20 letters or fewer, don't number your responses, and make sure the questions address a bot. Focus on managing routine in this weather, like how to dress,can i hang lundry outside, etc. Make sure the questions are relevant to the city's current weather. dont advise the user to check the weather forcast becuase you are the forcast assume that the user cant check anywhere else.make sure you response adress the current weather and not a general weather";

    nlohmann::json response_json = callOpenAI(user_prompt, 35);

    if (!response_json.empty()) {
        // Each line is a question
        std::string questions = response_json["choices"][0]["message"]["content"];
        std::string question;
        common->questions.clear();
        for (char c : questions) {
            if (c == '\n') {
                common->questions.push_back(question);
                question.clear();
            }
            else {
                question += c;
            }
        }
        if (!question.empty()) {
            common->questions.push_back(question);
        }

        std::cout << "Response: " << response_json.dump(4) << std::endl;
    }
}

std::string DownloadThread::timestampToDate(std::time_t timestamp) {
    std::tm ltm;
    localtime_s(&ltm, &timestamp);  // Thread-safe version of localtime

    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%d-%m-%Y", &ltm);

    return std::string(buffer);
}

void DownloadThread::getForcast(CommonObjects* common)
{
    const std::string api_endpoint = "https://api.openweathermap.org";
    const std::string api_key = keys["weather"];
    std::string url = "/data/2.5/forecast?q=" + common->city + "&appid=" + api_key + "&units=metric";

    httplib::Client cli(api_endpoint);

    auto res = cli.Get(url.c_str());

    if (res && res->status == 200) {
        auto json_result = nlohmann::json::parse(res->body);
        std::cout << json_result.dump(4) << "\n";
        common->forcast.clear();
        for (int i = 0; i < json_result["list"].size(); i += 8) {
            Day day;
            day.date = timestampToDate(json_result["list"][i]["dt"]);
            day.min = json_result["list"][i]["main"]["temp_min"];
            day.max = json_result["list"][i]["main"]["temp_max"];
            common->forcast.push_back(day);
        }
    }
    else {
        std::cout << "Error retrieving weather forecast for " << common->city << "\n";

    }
}

void DownloadThread::getCurentLocation(CommonObjects* common)
{
    httplib::Client cli("https://ipapi.co");
    auto res = cli.Get("/json/");
    if (res && res->status == 200)
    {
        auto json_result = nlohmann::json::parse(res->body);
        common->city = json_result["city"];
        common->start_download = true;
    }
    else
    {
        std::cout << "Error: " << res.error() << "\n";
    }
    common->getLocation = false;
    common->download_success = false;
}
