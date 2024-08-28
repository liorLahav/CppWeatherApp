

#include <iostream>
#include <thread>
#include "CommonObject.h"
#include "DrawThread.h"
#include "DownloadThread.h"
#include <fstream>



int main()
{ 
    // read the api keys from the file
    std::ifstream file("api_keys.txt");
    std::string line;
    if (file.is_open())
    {
		std::getline(file, line);
		file.close();
	}
    else
    {
		std::cerr << "Error opening file" << std::endl;
	}
    CommonObjects common;
    common.city = "London";
    DownloadThread down;
    auto down_th = std::jthread([&] {down(&common); });
    DrawThread draw;
    auto draw_th = std::jthread([&] {draw(common); });
    std::cout << "running...\n";
    down_th.join();
    draw_th.join();
}

