#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>

std::string Filename = "TestInput.txt";
//std::string Filename = "Input.txt";

struct Range
{
    int32_t Start = -1;
    int32_t End = -1;

    [[nodiscard]] bool IsEncapsulating(const Range& InOther) const
    {
        return InOther.Start >= Start
                && InOther.End <= End;
    }
    [[nodiscard]] int32_t HasEnyOverlapping(const Range& InOther) const
    {
        for (int32_t start = std::max(Start, InOther.Start); start <= std::min(End, InOther.End); ++start)
            return true;

        return false;
    }
};

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    int32_t numOverlaps = 0;
    while (!inputFile.eof())
    {
        std::getline(inputFile, inputLine);

        Range range1, range2;
        const int found = sscanf_s(inputLine.c_str(), "%d-%d,%d-%d", &range1.Start, &range1.End, &range2.Start, &range2.End);
        assert(found == 4);
        numOverlaps += range1.HasEnyOverlapping(range2) ? 1 : 0;
    }

    std::cout << "Number of overlaps: " << numOverlaps << std::endl;

    return 0;
}
