#pragma once

#include <atomic>
#include <string>
#include "nlohmann/json.hpp"
#include "City.h"
#include "Day.h"
#include <vector>

struct CommonObjects
{
	std::atomic_bool start_download = true;
	std::atomic_bool fetch_favorites = true;
	std::atomic_bool getLocation = false;
	std::atomic_bool download_success = true;
	std::atomic_bool favoritesRead = false;
	std::string city;
	bool isInFavorites = false;
	std::vector<City> fav_cities;
	std::vector<std::string> questions;
	int quetion = -1;
	City cityData;
	std::string review = "";
    std::vector<Day> forcast;
};

