#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    int cycle = 0;
    int32_t registerX = 1;
    int32_t sumSignalStrength = 0;
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
            cycle++;

            if (((cycle - 20) % 40) == 0)
            {
                std::cout << "Check " << cycle << " Value: " << registerX << std::endl;
                sumSignalStrength += cycle * registerX;
            }
            if (a == 1)
                registerX += value;
        }
    }
    
    std::cout << "Sum signal strength: " << sumSignalStrength << std::endl;
    
    return 0;
}
