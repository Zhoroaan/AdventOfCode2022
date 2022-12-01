

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//std::string filename = "TestInput.txt";
std::string Filename = "Input.txt";

int main(int argc, char* argv[])
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
    elfCalories.emplace_back();
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        if (inputLine.length() == 0)
            elfCalories.emplace_back();
        else
            elfCalories.back().emplace_back(std::atoi(inputLine.c_str()));
    }

    int32_t maxElfCarry = -1;

    for (auto& elf : elfCalories)
    {
        int32_t currentElfCalories = 0;
        for (const auto calories : elf)
            currentElfCalories += calories;
        
        maxElfCarry = std::max(maxElfCarry, currentElfCalories);
    }
    
    std::cout << "Max calories found: " << maxElfCarry  << std::endl;
    return 0;
}
