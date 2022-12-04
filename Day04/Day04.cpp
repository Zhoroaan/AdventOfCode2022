#include <fstream>
#include <iostream>
#include <locale>
#include <string>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

struct Range
{
    int32_t Start = -1;
    int32_t End = -1;

    [[nodiscard]] bool IsEncapsulating(const Range& InOther) const
    {
        return InOther.Start >= Start
                && InOther.End <= End;
    }
};

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
    int32_t numOverlaps = 0;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);

        Range range1, range2;
        int found = sscanf_s(inputLine.c_str(), "%d-%d,%d-%d", &range1.Start, &range1.End, &range2.Start, &range2.End);
        int breakHere = 2;
        numOverlaps += range1.IsEncapsulating(range2) || range2.IsEncapsulating(range1);
    }

    std::cout << "Number of overlaps: " << numOverlaps << std::endl;

    return 0;
}
