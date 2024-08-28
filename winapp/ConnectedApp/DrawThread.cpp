#include "DrawThread.h"
#include "GuiMain.h"
#include "../../shared/ImGuiSrc/imgui.h"
#include <io.h>
#include "tempShower.h"
#include "CommonObject.h"
#include "nlohmann/json.hpp"
#include "FavoritesTable.h"
#include <iostream>
#include "SearchBar.h"
#include "Week.h"

void drawFrameItems(void* common_ptr)
{
    CommonObjects* common = static_cast<CommonObjects*>(common_ptr);
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Fullscreen Window", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground);

    // make the window full screen
    ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

    // create a gradient background
    ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(
        ImVec2(0, 0),
        ImVec2(io.DisplaySize.x, io.DisplaySize.y),
        IM_COL32(0, 139, 139, 255),   // Top-left (Darker Turquoise)
        IM_COL32(0, 139, 139, 255),   // Top-right (Darker Turquoise)
        IM_COL32(64, 224, 208, 255),  // Bottom-right (Lighter Turquoise)
        IM_COL32(64, 224, 208, 255)   // Bottom-left (Lighter Turquoise)
    );

    // create headline
    ImGui::SetCursorPos(ImVec2(350, 50)); 
    ImGui::SetWindowFontScale(2.0f);
    ImGui::Text("Weather App");

    SearchBar searchBar(common);
    FavoritesTable favorites(common);
    if (common->fetch_favorites == false)
    {
		favorites.drawTable(common);
	}

    // check the satatus of the download
    ImGui::SetCursorPos(ImVec2(50, 150));
    if (common->start_download == true)
    {
        ImGui::Text("Downloading data...");
    }
    else if (common->download_success == false)
    {
		ImGui::Text("Error downloading data.\nPlease try again.");
	}
    else 
    {
        std::string cityN = common->cityData.name;
        // check if the city is in favorites
        common->isInFavorites = false;
        for (City city : common->fav_cities)
        {
            if (city.name == cityN)
            {
				common->isInFavorites = true;
				break;
			}
		}

        TempShower tempShower(common->cityData);
        tempShower.createTempWindow(favorites,common);
        Week week;
        if(common->forcast.size() > 0)
        week.drawWeek(common);
        ImGui::SetCursorPos(ImVec2(50, 150));
    }
    ImGui::End();
}

void DrawThread::operator()(CommonObjects& common)
{
    GuiMain(drawFrameItems, &common);
}