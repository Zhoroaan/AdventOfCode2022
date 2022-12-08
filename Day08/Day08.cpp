#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

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
    int count = 0;
    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {
            bool leftVisible = true;
            for (int cx = x - 1; cx >= 0; cx--)
                leftVisible &= heightData[y * width + x] > heightData[y * width + cx];
            bool rightVisible = true;
            for (int cx = x + 1; cx < width; cx++)
                rightVisible &= heightData[y * width + x] > heightData[y * width + cx];
            bool upVisible = true;
            for (int cy = y - 1; cy >= 0; cy--)
                upVisible &= heightData[y * width + x] > heightData[cy * width + x];
            bool downVisible = true;
            for (int cy = y + 1; cy < height; cy++)
                downVisible &= heightData[y * width + x] > heightData[cy * width + x];

            if (leftVisible || rightVisible || upVisible || downVisible)
                count++;
        }
    }
    const int edgeCount = width * 2 + height * 2 - 4;

    std::cout << "Visible trees found: " << count + edgeCount << std::endl;
    
    return 0;
}
