#include "FavoritesTable.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "GuiMain.h"
#include "../../shared/ImGuiSrc/imgui.h"
#include <io.h>
#include "tempShower.h"
#include "DownloadThread.h"
#include "City.h"


FavoritesTable::FavoritesTable(CommonObjects* common)
{
    // if the file was open dont open it again
    if (common->favoritesRead == true)
        return;

    std::ifstream file("favorites.txt");
    std::string line;
    // read from file the names of the favorite cities
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            City city;
            city.name = line;
            city.temperature = -1;
            city.description = "";
            common->fav_cities.push_back(city);
        }
        if (common->fav_cities.size() > 0) {
            common->fetch_favorites = true;
            common->favoritesRead = true;
        }
    }
    else
    {
        std::cerr << "Error opening file" << std::endl;
    }
    file.close();
}

void FavoritesTable::addEntry(City city, CommonObjects* common)
{
    if (!isCityInFavorites(city.name, common)) {
        std::ofstream file("favorites.txt", std::ios::app);
        if (file.is_open())
        {
            if (!isCityInFavorites(city.name, common)) {
                file << city.name << "\n";
            }
        }
        else
        {
            std::cerr << "Error opening file" << std::endl;
        }
        file.close();
    }
    common->fav_cities.push_back(city);
}

void FavoritesTable::removeEntry(std::string name, CommonObjects* common)
{
    std::string line;
    std::vector<std::string> entries;
    std::ofstream file("favorites.txt");
    int remove = -1;
    if (file.is_open())
    {
        for (int i = 0; i < common->fav_cities.size(); i++)
        {
            if (common->fav_cities[i].name != name)
            {
                file << common->fav_cities[i].name << "\n";
            }
            else {
                remove = i;
            }
        }
    }
    else
    {
        std::cerr << "Error opening file" << std::endl;
    }
    if (remove != -1)
        common->fav_cities.erase(common->fav_cities.begin() + remove);
    file.close();
}

void FavoritesTable::drawTable(CommonObjects* common) {
    // Set the position of the Favorites window
    ImGui::SetNextWindowPos(ImVec2(640, 100), ImGuiCond_Always);

    // Change the window background color to the darker turquoise color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(64 / 255.0f, 154 / 255.0f, 191 / 255.0f, 1.0f)); // Darker turquoise color

    // Set the title bar color to a darker turquoise
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(44 / 255.0f, 114 / 255.0f, 171 / 255.0f, 1.0f)); // Darker turquoise title bar color
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(44 / 255.0f, 114 / 255.0f, 171 / 255.0f, 1.0f)); // Same color when active
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(44 / 255.0f, 114 / 255.0f, 171 / 255.0f, 1.0f)); // Same color when collapsed

    // Start the ImGui window without the collapsing feature
    ImGui::Begin("Favorites", nullptr, ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    // Define the number of columns in the table
    if (ImGui::BeginTable("ExampleTable", 3))
    {
        // Setup the table headers with a slightly lighter shade of turquoise for contrast
        ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(64 / 255.0f, 154 / 255.0f, 191 / 255.0f, 1.0f)); // Slightly lighter turquoise header color
        ImGui::TableSetupColumn("City");
        ImGui::TableSetupColumn("Temperature");
        ImGui::TableSetupColumn("Description");

        // Finish setting up headers
        ImGui::TableHeadersRow();
        ImGui::PopStyleColor(); // Pop the header color



        // Add rows and columns with data
        for (int row = 0; row < common->fav_cities.size(); ++row)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; ++column)
            {
                ImGui::TableSetColumnIndex(column);
                switch (column)
                {
                case 0:
                    ImGui::Text(common->fav_cities[row].name.c_str());
                    break;
                case 1:
                {
                    // Format the temperature to two decimal places
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(2) << common->fav_cities[row].temperature;
                    ImGui::Text(stream.str().c_str());
                    break;
                }
                case 2:
                    ImGui::Text(common->fav_cities[row].description.c_str());
                    break;
                }
            }
        }

        // End the table
        ImGui::EndTable();
    }

    ImGui::End(); // End the ImGui window

    ImGui::PopStyleColor(4); // Pop the title bar colors and window background color
}

bool FavoritesTable::isCityInFavorites(std::string name, CommonObjects* common)
{
    for (City city : common->fav_cities)
    {
        if (city.name == name)
            return true;
    }
    return false;
}