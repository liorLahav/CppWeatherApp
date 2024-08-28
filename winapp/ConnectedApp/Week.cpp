#include "Week.h"
#include "Day.h"
#include "../ImGuiSrc/imgui.h"

void Week::drawWeek(CommonObjects* common) {
    // Set the background color of the window to white
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 

    // Start a new ImGui window for the week
    ImGui::Begin("Weekly Forecast", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus);

    // Set the desired initial window position
    ImGui::SetWindowPos(ImVec2(10, 600), ImGuiCond_Always);

    float totalWidth = 950;  
    ImGui::SetWindowSize(ImVec2(totalWidth, 0), ImGuiCond_Always); 

    // Calculate the width for each child window (day) to evenly distribute the space
    float childWidth = totalWidth / common->forcast.size();

    // Set background color for the days and add white borders
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(64 / 255.0f, 154 / 255.0f, 191 / 255.0f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f); 

    // Iterate through the forecast and draw each day as a square
    for (size_t i = 0; i < common->forcast.size(); ++i) {
        ImGui::BeginChild(std::to_string(i).c_str(), ImVec2(childWidth, 140), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        common->forcast[i].drawDay();  
        ImGui::EndChild();

        // Place the next day next to the previous one
        if (i < common->forcast.size() - 1) {
            ImGui::SameLine();
        }
    }

    // Restore the previous color and style
    ImGui::PopStyleVar(); 
    ImGui::PopStyleColor(2);  

    ImGui::End(); 

    // Restore the window background color to its previous state
    ImGui::PopStyleColor(); 
}
