#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include "City.h"
#include "CommonObject.h"
#ifndef FavoritesTable_H

#define FavoritesTable_H
class FavoritesTable {

	private:
		const std::string tableName = "yogev";
		std::ofstream file;
	public:
		FavoritesTable(CommonObjects* common);
		void addEntry(City city,CommonObjects* common);
		void removeEntry(std::string entry,CommonObjects* common);
		void drawTable(CommonObjects* common);
		bool isCityInFavorites(std::string name, CommonObjects* common);
};
#endif // FavoritesTable_H