#pragma once
#include <string>
#include <vector>
#include "UIContent.h"

class CSVDataLoader
{
public:
    CSVDataLoader();
    ~CSVDataLoader();

    static void LoadUICSV(std::vector<UIContent> &ui_data, std::string scene_name);

	static std::vector<std::string> split(std::string str, char del);
};

