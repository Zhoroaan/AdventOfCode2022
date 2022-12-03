#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

char FindCommon(const auto& pack1, const auto& pack2)
{
    for (auto itemType : pack1)
    {
        for (auto itemType2 : pack2)
        {
            if (itemType == itemType2)
                return itemType;
        }
    }
    return 0;
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

    std::string inputLine;
    int32_t scoreCounter = 0;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        
        if (inputLine.length() % 2 == 1)
        {
            std::cerr << "Invalid input line:  " << inputLine << std::endl;
            return 2;
        }
        const size_t count = inputLine.length() / 2;
        auto pack1 = inputLine.substr(0, count);
        auto pack2 = inputLine.substr(count, count);
        char common = FindCommon(pack1, pack2);
        if (common == 0)
        {
            std::cerr << "Found no common slot for:  " << pack1 << " and " << pack2 << std::endl;
            return 3;
        }
        const auto offset = std::isupper(common) ? 'A'  : 'a';
        scoreCounter +=  common - offset + (std::isupper(common) ? 27 : 1);
    }


    std::cout << "Sum: " << scoreCounter << std::endl;

    return 0;
}
