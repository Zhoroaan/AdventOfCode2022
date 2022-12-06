#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

int main(int argc, char* argv[])
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::vector<char> investigateChars;
        investigateChars.push_back(inputLine[0]);
        investigateChars.push_back(inputLine[1]);
        investigateChars.push_back(inputLine[2]);
        std::ranges::unique(investigateChars);
        
        for (int dataIndex = 3; dataIndex < static_cast<int>(inputLine.length()); ++dataIndex)
        {
            if (investigateChars.size() >= 4)
                investigateChars.erase(investigateChars.begin());
            investigateChars.push_back(inputLine[dataIndex]);
            std::vector<char> sortedList(investigateChars);
            std::ranges::sort(sortedList);
            auto duplicates = std::ranges::unique(sortedList);
            if (duplicates.empty())
            {
                std::cout << "First copy is: " << dataIndex + 1 << std::endl;
                break;
            }
        }
    }
    
    return 0;
}
