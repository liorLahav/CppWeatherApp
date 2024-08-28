#include "Day.h"
#include "../ImGuiSrc/imgui.h"

void Day::drawDay() {
    // Start a new ImGui child window for each day, within the Week container
    ImGui::BeginChild("Day Container", ImVec2(150, 120), false,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoScrollbar);

    // Set the font scale to make the date smaller
    ImGui::SetWindowFontScale(1.2f);

    // Calculate the size of the date text
    ImVec2 dateSize = ImGui::CalcTextSize(date.c_str());

    // Calculate the position to center the date horizontally
    ImVec2 windowSize = ImGui::GetWindowSize();
    float datePosX = (windowSize.x - dateSize.x) * 0.5f;
    ImGui::SetCursorPos(ImVec2(datePosX, 10)); // Position the date at the top with some padding
    ImGui::Text("%s", date.c_str());

    // Reset font scale for the temperature
    ImGui::SetWindowFontScale(1.0f);

    // Calculate the size of the min and max temperature texts
    ImVec2 minSize = ImGui::CalcTextSize("Min: 00.00");
    ImVec2 maxSize = ImGui::CalcTextSize("Max: 00.00");

    // Including padding between lines
    float totalTempHeight = minSize.y + maxSize.y + 10; 

    // Calculate the position to center temperatures vertically and horizontally
    float tempStartY = (windowSize.y - totalTempHeight) * 0.5f;

    // Center min temperature horizontally
    float minTempPosX = (windowSize.x - minSize.x) * 0.5f;
    ImGui::SetCursorPos(ImVec2(minTempPosX, tempStartY));
    ImGui::Text("Min: %.2f", min);

    // Center max temperature horizontally
    float maxTempPosX = (windowSize.x - maxSize.x) * 0.5f;
    ImGui::SetCursorPos(ImVec2(maxTempPosX, ImGui::GetCursorPosY() + 10)); // Small padding between min and max
    ImGui::Text("Max: %.2f", max);

    ImGui::EndChild(); // End the child window
}
