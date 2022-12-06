#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int argc, char* argv[])
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::vector<char> investigateChars;
        investigateChars.reserve(14);
        for (int startIndex = 0; startIndex < 13; ++startIndex)
            investigateChars.push_back(inputLine[startIndex]);
        
        std::ranges::unique(investigateChars);
        
        for (int dataIndex = 13; dataIndex < static_cast<int>(inputLine.length()); ++dataIndex)
        {
            investigateChars.push_back(inputLine[dataIndex]);
            if (investigateChars.size() > 14)
                investigateChars.erase(investigateChars.begin());
            
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