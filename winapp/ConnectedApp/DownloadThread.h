#pragma once

#include "CommonObject.h"
#include "City.h"
#include <map>

class DownloadThread
{
public:
	void operator()(CommonObjects* common);
private:
	std::map<std::string, std::string> keys;
	void getFavCitiesTemp(CommonObjects* common);
	void SetCityUrl(std::string_view new_url);
	void getCityTemp(CommonObjects& common);
	void getKeys();
	void getLocationDetails(CommonObjects* common);
	void followupQuetion(CommonObjects* common);
	void createFollowUpQuetion(CommonObjects* common);
	auto openai_request(const std::string& prompt, int max_tokens);
	std::string timestampToDate(std::time_t timestamp);
	void getForcast(CommonObjects* common);
	std::string _download_url;
	std::string city;
	nlohmann::json callOpenAI(const std::string& user_prompt, int max_tokens);
	nlohmann::json callWeatherAPI(const std::string& city);
	void getCurentLocation(CommonObjects* common);
};

