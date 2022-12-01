

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

int main(int InArgc, char* INArgv[])
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to find file " << Filename << std::endl;
        return 1;
    }

    std::string inputLine;
    std::vector<std::vector<int32_t>> elfCalories;
    elfCalories.reserve(100);
    elfCalories.emplace_back();
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        if (inputLine.length() == 0)
            elfCalories.emplace_back();
        else
            elfCalories.back().emplace_back(std::atoi(inputLine.c_str()));
    }

    std::vector<int32_t> elfCarryWeights;
    elfCarryWeights.reserve(100);

    for (auto& elf : elfCalories)
    {
        int32_t currentElfCalories = 0;
        for (const auto calories : elf)
            currentElfCalories += calories;
        
        elfCarryWeights.emplace_back(currentElfCalories);
    }
    // Sort descending
    auto descendingIntegerSort = [](const int32_t& InA, const int32_t& InB) { return InA > InB; };
    std::ranges::sort(elfCarryWeights, descendingIntegerSort);


    int32_t calCounter = 0;
    constexpr int32_t topNumberOfElves = 3;
    for (int32_t a = 0; a < topNumberOfElves; ++a)
    {
        calCounter += elfCarryWeights[a];
    }
    
    std::cout << "Max calories found: " << calCounter  << std::endl;
    return 0;
}
