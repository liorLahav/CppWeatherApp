#pragma once
#include "GuiMain.h"
#include <io.h>
#include <string>
#include <atomic>
#include "../../shared/ImGuiSrc/imgui.h"
#include "FavoritesTable.h"
#include "CommonObject.h"
#include <string>
#include "City.h"


class TempShower {
private:
    std::string city;
    std::string description;
    float temp;

public:
    TempShower(City city);
    void createTempWindow(FavoritesTable& favorites, CommonObjects* common);
};