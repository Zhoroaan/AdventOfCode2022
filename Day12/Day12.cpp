#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>

//std::string Filename = "TestInput.txt";
std::string Filename = "Input.txt";

struct Position
{
    Position (int InX = 0, int InY = 0)
        : X(InX), Y(InY)
    {}

    int X = 0;
    int Y = 0;
    bool operator==(const Position& InVector) const = default;

    Position operator+(const Position& InOther) const
    {
        return Position{X + InOther.X, Y + InOther.Y};
    }
    
    Position operator<(const Position& InOther) const
    {
        return GetHash() < InOther.GetHash();
    }

    [[nodiscard]] int32_t GetHash() const
    {
        const int hash = Y;
        return (hash << 16) + X;
    }
};

struct Leaf
{
    Leaf(int32_t InCost = -1, const Position& InPosition = Position(), const Position& InWalkedFrom = Position())
        : Cost(InCost), CurrentPosition(InPosition), WalkedFrom(InWalkedFrom)
    {}
    int32_t Cost;
    Position CurrentPosition;
    Position WalkedFrom;
};

std::vector<Position> GetValidMoves(const Position& InPos, const std::vector<std::string>& InHeightData, const std::vector<Leaf>& InClosed, const std::vector<Leaf>& InOpen)
{
    std::vector<Position> returnVector;

    std::array<Position, 4> dirExplore = {Position{-1, 0}, Position{0, 1}, Position{1, 0}, Position{0, -1}};

    for (const Position& direction : dirExplore)
    {
        const auto pos = InPos + direction;
        if (pos.X < 0 || pos.Y < 0 || pos.X >= InHeightData[0].length() || pos.Y >= InHeightData.size())
            continue;

        bool found = false;
        for (const auto& leaf : InClosed)
        {
            if (leaf.CurrentPosition == pos)
                found = true;
        }
        for (const auto& leaf : InOpen)
        {
            if (leaf.CurrentPosition == pos)
                found = true;
        }
        if (found)
            continue;

        const auto currentHeight = InHeightData[InPos.Y][InPos.X];
        const auto newHeight = InHeightData[pos.Y][pos.X];
        if (newHeight <= currentHeight || newHeight - currentHeight <= 1)
            returnVector.push_back(pos);
    }
    
    return returnVector;
}

std::ostream& operator<<(std::ostream& InOStream, const Position& InPos)
{
    return InOStream << "Position X: " << InPos.X << " Y: " << InPos.Y;
}


char GetArrowFromLocations(const Position& InCurrentPosition, const Position& InPreviousPosition)
{
    if (InPreviousPosition.X < InCurrentPosition.X)
        return '>';
    if (InPreviousPosition.X > InCurrentPosition.X)
        return '<';
    if (InPreviousPosition.Y < InCurrentPosition.Y)
        return 'v';
    if (InPreviousPosition.Y > InCurrentPosition.Y)
        return '^';

    return '-';
}

int main(int /*InArgc*/, char* /*InArgv[]*/)
{
    std::ifstream inputFile;

    inputFile.open(Filename);
    std::string inputLine;
    std::vector<std::string> data;
    Position start, end;
    while (inputFile.is_open() && !inputFile.eof())
    {
        std::getline(inputFile, inputLine);
        for (int a = 0; a < inputLine.length(); ++a)
        {
            char& height = inputLine[a];
            if (height == 'S')
            {
                start = {a, static_cast<int>(data.size())};
                height = 'a';
            }
            if (height == 'E')
            {
                end = {a, static_cast<int>(data.size())};
                height = 'z';
            }
        }
        data.emplace_back(inputLine);
    }

    std::vector<Leaf> open;
    std::vector<Leaf> closed;

    open.emplace_back(0, start);

    bool foundEnd = false;
    int32_t endCost = 0;
    while (!open.empty() && !foundEnd)
    {
        const auto currentPos = open[0].CurrentPosition;
        const auto currentCost = open[0].Cost;

        std::vector<Position> directionsToExplore = GetValidMoves(currentPos, data, closed, open);
        closed.emplace_back(open[0]);
        open.erase(open.begin());
        
        for (const auto& newPosition : directionsToExplore)
        {
            if (newPosition == end)
            {
                foundEnd = true;
                closed.emplace_back(currentCost + 1, newPosition, currentPos);
                endCost = currentCost + 1;
            }
            else
                open.emplace_back(currentCost + 1, newPosition, currentPos);
        }

        
        std::ranges::sort(open, [] (const auto& InA, const auto& InB) { return InA.Cost < InB.Cost;});
        int breakHere = 2;
    }

    Position searcher = end;
    std::vector<std::string> outputArea;
    for (auto& inputLine : data)
    {
        std::string temp;
        for (auto line : inputLine)
        {
            temp += '.';
        }
        outputArea.push_back(temp);
    }
    
    while (searcher != start)
    {
        auto found = std::ranges::find_if(closed, [&searcher](const auto& InLeaf) {return InLeaf.CurrentPosition == searcher;});
        char arrow= GetArrowFromLocations(found->CurrentPosition, found->WalkedFrom);
        searcher = found->WalkedFrom;
        outputArea[searcher.Y][searcher.X] = arrow;
    }

    for (auto& outputArea : outputArea)
    {
        std::cout << outputArea << std::endl;
    }
    
    std::cout << "Number of steps: " << endCost << std::endl;
    return 0;
}
