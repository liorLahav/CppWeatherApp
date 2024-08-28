#include "GuiMain.h"
#include <io.h>
#include "TempShower.h"
#include "DownloadThread.h"
#include "FavoritesTable.h"


TempShower::TempShower(City city)
{
    this->city = city.name;
    this->description = city.description;
    this->temp = city.temperature;
}

void TempShower::createTempWindow(FavoritesTable& favorites, CommonObjects* common)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Temperature Window", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoScrollbar);

    // Set the window position and size
    ImGui::SetWindowPos(ImVec2(10, 150), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(600, 500), ImGuiCond_Always);

    ImGui::SetWindowFontScale(1.8f);

    // Center the text
    ImGui::SetCursorPos(ImVec2(50, 50));
    ImGui::Text(city.c_str());
    ImGui::Text("Description: %s", description.c_str());
    ImGui::Text("Temperature: %.2f°", temp);

    // Restore the original position of Refresh and Favorites buttons
    ImGui::SetCursorPos(ImVec2(5, 180));

    ImGui::SetWindowFontScale(1.2f);

    // Push the style settings for the buttons
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(64 / 255.0f, 154 / 255.0f, 191 / 255.0f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(84 / 255.0f, 174 / 255.0f, 211 / 255.0f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(44 / 255.0f, 134 / 255.0f, 171 / 255.0f, 1.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f); 

    // Refresh Button
    ImGui::SetNextItemWidth(75);  // Increase the button width
    if (ImGui::Button("Refresh"))
    {
        common->start_download = true;
        common->fetch_favorites = true;
    }

    ImGui::SameLine();

    // Add/Remove from Favorites Button
    ImGui::SetNextItemWidth(150);  // Increase the button width
    if (common->isInFavorites)
    {
        if (ImGui::Button("Remove from favorites"))
        {
            favorites.removeEntry(city, common);
            common->isInFavorites = false;
        }
    }
    else
    {
        if (ImGui::Button("Add to favorites"))
        {
            favorites.addEntry(common->cityData, common);
            common->isInFavorites = true;
        }
    }

    // Place the Exit button next to the Favorites button
    ImGui::SameLine();
    ImGui::SetNextItemWidth(75);  // Set the button width
    if (ImGui::Button("Exit"))
    {
        exit(0);
    }

    // Display the review content from common->review
    ImGui::SetCursorPos(ImVec2(10, 220));
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetNextItemWidth(380);
    ImGui::TextWrapped("%s", common->review.c_str());  // Wrapped text to handle long reviews

    if (common->review.size() > 0) {
        // Position the question buttons below the text
        for (int i = 0; i < common->questions.size(); i++)
        {
            if (i > 0 && i % 2 != 0)  // After every second button, start a new line
            {
                ImGui::SameLine();
            }
            ImGui::SetWindowFontScale(1.0f);
            ImGui::SetCursorPos(ImVec2(10 + (i % 2) * 290, 340 + (i / 2) * 40));  // Adjust position for each button
            ImGui::SetNextItemWidth(120);  // Set the button width

            if (ImGui::Button(common->questions[i].c_str()))
            {
                common->quetion = i;
            }
        }
    }

    ImGui::PopStyleVar(); // Pop the rounding style
    ImGui::PopStyleColor(3); // Pop the button color styles

    ImGui::End();
}


