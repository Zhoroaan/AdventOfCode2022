#include <fstream>
#include <iostream>
#include <locale>
#include <string>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

char FindCommon(const auto& InGroup1, const auto& InGroup2, const auto& InGroup3)
{
    for (auto itemType : InGroup1)
    {
        for (auto itemType2 : InGroup2)
        {
            if (itemType == itemType2)
            {
                for (auto itemType3 : InGroup3)
                {
                    if (itemType == itemType3)
                        return itemType;
                }
            }
        }
    }
    return 0;
}

auto CalculateScore(char InItemType)
{
    const auto offset = std::isupper(InItemType) ? 'A'  : 'a';
    return InItemType - offset + (std::isupper(InItemType) ? 27 : 1);
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to find file " << Filename << std::endl;
        return 1;
    }

    int32_t scoreCounter = 0;
    while (!inputFile.eof())
    {
        std::string group1;
        std::getline(inputFile, group1);
        if (group1.length() == 0)
            continue;
        
        std::string group2, group3;
        std::getline(inputFile, group2);
        std::getline(inputFile, group3);
        
        char common = FindCommon(group1, group2, group3);
        if (common == 0)
        {
            std::cerr << "Found no common slot for:  " << group1 << ", " << group2 << " and " << group3 << std::endl;
            return 3;
        }
        scoreCounter += CalculateScore(common);
    }

    std::cout << "Sum: " << scoreCounter << std::endl;

    return 0;
}
