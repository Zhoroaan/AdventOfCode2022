

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

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
    std::vector<std::vector<int32_t>> elfCalories;
    elfCalories.reserve(100);
    elfCalories.emplace_back();
    std::vector<std::pair<char, char>> inputData;
    std::map<char, char> inputMapping =
        {
        {'X', 'A'},
        {'Y', 'B'},
        {'Z', 'C'}
        };
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        
        if (inputLine.length() != 3)
        {
            std::cerr << "Invalid input line:  " << inputLine << std::endl;
            return 2;
        }

        char pickedResponse = inputLine[2];
        if (!inputMapping.contains(pickedResponse))
        {
            std::cerr << "Invalid response action: " << pickedResponse << std::endl;
            return 3;
        }
        inputData.emplace_back(std::pair<char, char>{inputLine[0], inputMapping[pickedResponse]});
    }

    std::map<char, int32_t> scoreMapping =
        {
        {'A', 1},
        {'B', 2},
        {'C', 3}
        };

    std::map<char, char> winMapping =
        {
        {'A', 'B'},
        {'B', 'C'},
        {'C', 'A'}
        };

    int32_t score = 0;
    for (const auto& game : inputData)
    {
        score += scoreMapping[game.second];

        if (game.first == game.second)
        {
            score += 3;
        }
        else
        {
            score += winMapping[game.first] == game.second ? 6 : 0;
        }
    }

    std::cout << "Score: " << score << std::endl;

    return 0;
}
