#include "SearchBar.h"

// Set the locale to Hebrew (Israel)
SearchBar::SearchBar(CommonObjects* common)
{
    ImGuiIO& io = ImGui::GetIO();
    static char search_buffer[256] = "";

    // Begin the ImGui window for the search bar
    ImGui::Begin("Search Bar", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoScrollbar);

    // Set the position and size of the search bar
    ImGui::SetNextItemWidth(400);

    // Change the color of the search bar background to the darker turquoise color
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(64 / 255.0f, 154 / 255.0f, 191 / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(84 / 255.0f, 174 / 255.0f, 211 / 255.0f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(44 / 255.0f, 134 / 255.0f, 171 / 255.0f, 1.0f)); 

    // Change the text color of the input text to light gray
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Light gray text color

    // Change the watermark (disabled text) color to white
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White watermark text color

    // Change the watermark text
    ImGui::InputTextWithHint("##search", "Type to search...", search_buffer, IM_ARRAYSIZE(search_buffer));

    // Pop the color styles
    ImGui::PopStyleColor(5);

    // Set the position and size of the window
    ImGui::SetWindowPos(ImVec2(2, 90), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(io.DisplaySize.x / 2, 20), ImGuiCond_Always);

    // Create the Search
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(64 / 255.0f, 154 / 255.0f, 191 / 255.0f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(84 / 255.0f, 174 / 255.0f, 211 / 255.0f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(44 / 255.0f, 134 / 255.0f, 171 / 255.0f, 1.0f)); 

    // Set the button size
    ImGui::SetNextItemWidth(io.DisplaySize.x * 0.5f * 0.2f);

    if (ImGui::Button("Search"))
    {
        // Get the search text
        std::string search_text = search_buffer;
        // Convert to upper case
        std::transform(search_text.begin(), search_text.end(), search_text.begin(), ::toupper);
        common->city = search_text;
        common->start_download = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("My Location"))
    {
        common->getLocation = true;
        common->start_download = true;
    }

    // Pop the button color styles
    ImGui::PopStyleColor(3);

    ImGui::End();
}
