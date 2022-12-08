#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    int32_t width = 0, height = 0;
    std::vector<int32_t> heightData;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        width = static_cast<int32_t>(inputLine.length());

        for (const char t : inputLine)
            heightData.push_back(t - '0');
        
        height++;
    }
    uint64_t maxScore = 0;
    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {
            // Add the number of trees I can see over with a tree beyond them
            uint64_t leftVisible = 1;
            for (int cx = x - 1; cx >= 1 && heightData[y * width + x] > heightData[y * width + cx]; cx--)
                leftVisible++;
            
            uint64_t rightVisible = 1;
            for (int cx = x + 1; cx < width - 1 && heightData[y * width + x] > heightData[y * width + cx]; cx++)
                rightVisible++;
            
            uint64_t upVisible = 1;
            for (int cy = y - 1; cy >= 1 && heightData[y * width + x] > heightData[cy * width + x]; cy--)
                upVisible++;
            
            uint64_t downVisible = 1;
            for (int cy = y + 1; cy < height - 1 && heightData[y * width + x] > heightData[cy * width + x]; cy++)
                downVisible++;

            maxScore = std::max(maxScore, leftVisible * rightVisible * upVisible * downVisible);
        }
    }

    std::cout << "Max Score found: " << maxScore << std::endl;
    
    return 0;
}
