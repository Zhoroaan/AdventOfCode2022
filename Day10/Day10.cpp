#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    int cycle = 0;
    int32_t registerX = 1;
    std::array<char, 240> crtDraw{'E'};
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        std::stringstream l(inputLine);
        std::string instruction;
        int32_t value = std::numeric_limits<int32_t>::min();
        l >> instruction >> value;
        int instructionCount = instruction[0] == 'a' ? 2 : 1;
        for (int a = 0; a <instructionCount; ++a)
        {
            crtDraw[cycle] = std::abs(registerX - cycle % 40) <= 1 ?  '#' : '.';
            cycle++;
            if (a == 1)
                registerX += value;
        }
    }

    for (int y = 0; y < 6; ++y)
    {
        for (int x = 0; x < 40; ++x)
            std::cout << crtDraw[x + y * 40];
        std::cout << std::endl;
    }
    
    return 0;
}
