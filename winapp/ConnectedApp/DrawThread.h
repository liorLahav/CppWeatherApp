#pragma once
#include "CommonObject.h"
#include "FavoritesTable.h"

class DrawThread
{
private:
public:
	void operator()(CommonObjects& common);
	//void DrawAppWindow();
};

