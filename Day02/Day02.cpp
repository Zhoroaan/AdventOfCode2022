#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

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
    std::map<char, char> loseMapping =
        {
        {'A', 'C'},
        {'B', 'A'},
        {'C', 'B'}
        };

    std::map<char, char> winMapping =
    {
        {'A', 'B'},
        {'B', 'C'},
        {'C', 'A'}
    };
    
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        
        if (inputLine.length() != 3)
        {
            std::cerr << "Invalid input line:  " << inputLine << std::endl;
            return 2;
        }

        char pickedResponse = 0;
        if (inputLine[2] == 'X')
            pickedResponse = loseMapping[inputLine[0]];
        if (inputLine[2] == 'Y')
            pickedResponse = inputLine[0];
        if (inputLine[2] == 'Z')
            pickedResponse = winMapping[inputLine[0]];
        
        inputData.emplace_back(std::pair<char, char>{inputLine[0], pickedResponse});
    }

    std::map<char, int32_t> scoreMapping =
        {
        {'A', 1},
        {'B', 2},
        {'C', 3}
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
